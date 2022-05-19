#include "transport.h"
#include "packet_data.h"
#include <arpa/inet.h>
#include <bits/types/FILE.h>
#include <bits/types/struct_timespec.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <poll.h>
#include <string>
#include <strings.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

#include <algorithm>
#include <iterator>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>

Transport::Transport(int argc, char *argv[]) {
  if (argc < 5) {
    fprintf(stderr, "Invalid number of arguments (4 required, given %d)\n",
            argc - 1);
    exit(EXIT_FAILURE);
  }

  std::string ip_address_string(argv[1]);
  port = atoi(argv[2]);
  filename = std::string(argv[3]);
  filemode = "w";
  size = atoi(argv[4]);

  // fp = fopen(filename.c_str(), filemode.c_str());
  file_descriptor = open(filename.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);

  // std::cout << argc << " " << ip_address_string << " " << port << " "
  //           << filename << " " << size << "\n";

  if (port < 1024 || port > 65535) {
    fprintf(stderr,
            "Range for ephemeral ports should be 1024-65535 (entered %d)\n",
            port);
    exit(EXIT_FAILURE);
  }

  if (size > 10000000) {
    fprintf(stderr,
            "Size is too large. Enter number between 0-10000000 bytes (entered "
            "%d).\n",
            size);
    exit(EXIT_FAILURE);
  }

  int result = inet_pton(AF_INET, ip_address_string.c_str(), &ipv4_address);
  if (result == 0) {
    fprintf(stderr,
            "<inet_pton>: src does not contain a character string representing "
            "a valid network address in the specified address family.\n");
    exit(EXIT_FAILURE);
  } else if (result == -1) {
    perror("<inet_pton>");
    exit(EXIT_FAILURE);
  }

  window = std::deque<PacketData>(DATAGRAMS_IN_WINDOW,
                                  PacketData(BYTES_IN_DATAGRAM, 0));

  for (int i = 0; i < window.size(); i++) {
    window[i].start_byte = i * BYTES_IN_DATAGRAM;
  }

  saved_bytes = 0;
  last_index = DATAGRAMS_IN_WINDOW;
  head = tail = 0;
}

void Transport::print_window() {
  int i = 0;
  for (auto datagram : window) {
    printf("[%d]<%s>[start: %u size: %u] ", i++,
           datagram.received ? "received" : "waiting ", datagram.start_byte,
           datagram.size);
    for (uint8_t byte : datagram.bytes) {
      printf("%x ", byte);
    }
    printf("\n");
  }
  printf("\n");
}

void Transport::initialize_socket() {
  create_socket();
  bind_socket_to_port();
}

void Transport::send_requests() {
  for (PacketData &datagram : window) {
    if (datagram.start_byte < size && !datagram.received) {
      uint trailing_bytes = size - datagram.start_byte;
      // printf("start_byte: %d last_index: %d datagram_size: %d size: %d "
      //        "trailing_bytes: %d\n",
      //  datagram.start_byte, last_index, datagram.size, size,
      //  trailing_bytes);

      datagram.size = std::min(datagram.size, trailing_bytes);

      send_datagram(datagram.start_byte, datagram.size);
    }
  }
}

void Transport::listen_for_responses() {
  int timeout = 1 * 1000;
  struct timespec begin, end;

  struct pollfd fds {
    .fd = sockfd, .events = POLLIN, .revents = 0
  };

  while (timeout > 0) {
    clock_gettime(CLOCK_REALTIME, &begin);
    int ready_fds = poll(&fds, 1, timeout);
    clock_gettime(CLOCK_REALTIME, &end);

    if (ready_fds == -1) {
      perror("poll");
      exit(EXIT_FAILURE);
    } else if (ready_fds > 0) {
      read_socket();
    }

    timeout -= (end.tv_sec - begin.tv_sec) * 1000 +
               (end.tv_nsec - begin.tv_nsec) / 1000000;
  }
}

void Transport::read_socket() {
  char buffer[IP_MAXPACKET + 1];
  struct sockaddr_in sender;
  socklen_t address_length = sizeof(sender);

  char sender_ip_string[20];

  ssize_t received_bytes =
      recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *)&sender,
               &address_length);
  if (received_bytes == -1) {
    perror("<recvfrom>");
    exit(EXIT_FAILURE);
  }

  if (sender_is_valid(sender)) {
    // std::cout << "sender is valid\n";
    store_received_data(buffer, received_bytes);
  }
}

void Transport::store_received_data(char buffer[IP_MAXPACKET + 1],
                                    ssize_t received_bytes) {
  std::string received_message(buffer);

  // std::cout << "RECEIVED MESSAGE: " << received_message << "\n";

  std::stringstream sstream(buffer);
  std::string word;

  int header_size = 0;
  std::getline(sstream, word, ' ');
  header_size += word.size() + 1;
  std::getline(sstream, word, ' ');
  header_size += word.size() + 1;
  uint datagram_start = std::stoi(word);
  std::getline(sstream, word, '\n');
  header_size += word.size() + 1;
  uint datagram_size = std::stoi(word);

  if (received_bytes - header_size != datagram_size) {
    fprintf(stderr,
            "Number of bytes received does not match required number of bytes "
            "(%zd/%d) %d.\n",
            received_bytes, datagram_size, header_size);
  }

  uint i = get_data_start_index(buffer);
  if (i < 0) {
    // std::cout << "<get_data_start_index>Something is not yes.\n";
    return;
  }
  // uint i = 0;
  // for (; i < IP_MAXPACKET + 1; i++) {
  //   if (buffer[i] == '\n')
  //     break;
  // }

  // std::cout << i << "\n";

  // std::cout << datagram_start << " " << datagram_size << "\n";

  int datagram_index = get_datagram_index(datagram_start, datagram_size);

  if (datagram_index < 0) {
    // std::cout << "<get_datagram_index>Something is not yes.\n";
    // exit(0);
    return;
  }

  if (window[datagram_index].start_byte == datagram_start &&
      window[datagram_index].size == datagram_size) {
    for (int j = 0; j < datagram_size; j++) {
      window[datagram_index].bytes[j] = buffer[j + i];
    }
    window[datagram_index].received = true;
  }

  write_window_prefix_to_file();
}

uint Transport::get_data_start_index(char buffer[IP_MAXPACKET + 1]) {
  uint i = 0;
  for (; i < IP_MAXPACKET + 1; i++) {
    if (buffer[i] == '\n')
      return i + 1;
  }
  return -1;
}

bool Transport::sender_is_valid(struct sockaddr_in sender) {
  std::cout << "sender: " << sender.sin_addr.s_addr << " " << sender.sin_port
            << " " << ((sender.sin_addr.s_addr == ipv4_address.s_addr) && (sender.sin_port == htons(port))) << "\n";
  return sender.sin_addr.s_addr == ipv4_address.s_addr &&
         sender.sin_port == htons(port);
}

void Transport::write_window_prefix_to_file() {
  int prefix_length = calculate_received_datagrams_prefix_length();

  // print_window();
  write_to_file(prefix_length);
  pop_window_prefix(prefix_length);
  push_window_suffix(prefix_length);

  if (prefix_length > 0) {
    std::cout << "Saved: " << saved_bytes << "/" << size << " bytes\n";
  }
}

int Transport::calculate_received_datagrams_prefix_length() {
  int prefix_length = 0;
  while (window[prefix_length].received && prefix_length < window.size()) {
    prefix_length++;
  }

  return prefix_length;
}

void Transport::write_to_file(int count) {
  int retval;
  for (int i = 0; i < count; i++) {
    retval = write(file_descriptor, &window[i].bytes[0], window[i].size);
    saved_bytes += retval;
  }
}

void Transport::pop_window_prefix(int count) {
  for (int i = 0; i < count; i++) {
    window.pop_front();
  }
}

void Transport::push_window_suffix(int count) {
  for (int i = 0; i < count; i++) {
    window.push_back(
        PacketData(BYTES_IN_DATAGRAM, (last_index + i) * BYTES_IN_DATAGRAM)); //
    // std::cout << "PUSHING: " << (last_index + i) * BYTES_IN_DATAGRAM << "\n";
  }
  last_index += count;
}

int Transport::get_datagram_index(uint datagram_start, uint datagram_size) {
  // printf("GET_DATAGRAM_INDEX ds: %d sb: %d bid: %d index: %d\n",
  // datagram_start, saved_bytes, BYTES_IN_DATAGRAM, ((int)datagram_start -
  // (int)saved_bytes) / (int)BYTES_IN_DATAGRAM);
  return ((int)datagram_start - (int)saved_bytes) / (int)BYTES_IN_DATAGRAM;
}

void Transport::send_datagram(int start, int end) {
  // Maybe can be done in a more elegant way
  char buffer[18];
  sprintf(buffer, "GET %d %d\n", start, end);
  // printf("GET %d %d\n", start, end);
  ssize_t buffer_len = strlen(buffer);

  struct sockaddr_in recipient;
  bzero(&recipient, sizeof(recipient));
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(port);
  recipient.sin_addr = ipv4_address;

  ssize_t bytes_sent = sendto(sockfd, buffer, buffer_len, 0,
                              (struct sockaddr *)&recipient, sizeof(recipient));
  if (bytes_sent == -1) {
    perror("<sendto>");
    exit(EXIT_FAILURE);
  }
  if (bytes_sent != buffer_len) {
    fprintf(stderr, "<sendto>: Some bytes haven't been sent (sent %zd/%zd).\n",
            bytes_sent, buffer_len);
  }
}

void Transport::create_socket() {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd < 0) {
    perror("<socket>");
    exit(EXIT_FAILURE);
  }
}

void Transport::bind_socket_to_port() {
  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(32456);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) <
      0) {
    perror("<bind>");
    exit(EXIT_FAILURE);
  }
}
