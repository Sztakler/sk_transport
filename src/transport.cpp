#include "transport.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <strings.h>

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

void Transport::send_data() {
  
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
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  std::cout << "sockfd: " << sockfd << "\n";
  if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) <
      0) {
    perror("<bind>");
    exit(EXIT_FAILURE);
  }
}
