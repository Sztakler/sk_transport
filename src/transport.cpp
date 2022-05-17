#include "transport.h"
#include <arpa/inet.h>
#include <bits/types/struct_timespec.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <poll.h>
#include <strings.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>

Transport::Transport(int argc, char *argv[]) {
  if (argc < 5) {
    fprintf(stderr, "Invalid number of arguments (4 required, given %d)\n",
            argc - 1);
    exit(EXIT_FAILURE);
  }

  std::string ip_address_string(argv[1]);
  port = atoi(argv[2]);
  filename = std::string(argv[3]);
  size = atoi(argv[4]);

  std::cout << argc << " " << ip_address_string << " " << port << " "
            << filename << " " << size << "\n";

  if (port < 1024 || port > 65535) {
    fprintf(stderr,
            "Ranger for ephemeral ports should be 1024-65535 (entered %d)\n",
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
}

void Transport::initialize_socket() {
  create_socket();
  bind_socket_to_port();
}

void Transport::send_requests() { send_datagram(0, 100); }

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

  ssize_t datagram_size = recvfrom(sockfd, buffer, IP_MAXPACKET, 0,
                                   (struct sockaddr *)&sender, &address_length);

  if (datagram_size == -1) {
    perror("<recvfrom>");
    exit(EXIT_FAILURE);
  }

  std::cout << sender.sin_addr.s_addr << " " << ipv4_address.s_addr << "\n";

  std::string received_message(buffer);
  std::cout << "Received message: "<< received_message << "\n";
}

void Transport::send_datagram(int start, int end) {
  // Maybe can be done in a more elegant way
  char buffer[18];
  sprintf(buffer, "GET %d %d\n", start, end);
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
  std::cout << "sockfd: " << sockfd << "\n";
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
  std::cout << "sockfd: " << sockfd << "\n";
  if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) <
      0) {
    perror("<bind>");
    exit(EXIT_FAILURE);
  }
}
