#include "transport.h"
#include "transport.h"
#include <arpa/inet.h>
#include <iostream>

Transport::Transport(int argc, char* argv[]) {
    if (argc < 5) {
    fprintf(stderr, "Invalid number of arguments (4 required, given %d)\n",
            argc - 1);
    exit(EXIT_FAILURE);
  }

  std::string ip_address_string(argv[1]);
  int port = atoi(argv[2]);
  std::string filename(argv[3]);
  uint size = atoi(argv[4]);

  std::cout << argc << " " << ip_address_string << " " << port << " "
            << filename << " " << size << "\n";

  struct in_addr ipv4_address;
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


  if (port < 1024 || port > 65535) {
    fprintf(stderr, "Ranger for ephemeral ports should be 1024-65535 (entered %d)\n", port);
    exit(EXIT_FAILURE);
  }

  if (size > 10000000) {
    fprintf(stderr, "Size is too large. Enter number between 0-10000000 bytes (entered %d).\n", size);
    exit(EXIT_FAILURE);
  }
}


void Transport::create_socket(){}

void Transport::bind_socket_to_port(){}

