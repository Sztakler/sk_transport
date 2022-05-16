#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "transport.h"

int main(int argc, char *argv[]) {

  Transport transport(argc, argv);

  transport.create_socket();
  transport.bind_socket_to_port();

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    fprintf(stderr, "socket error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(32345);
  inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

  char *message = "Hello server!";
  ssize_t message_len = strlen(message);
  if (sendto(sockfd, message, message_len, 0,
             (struct sockaddr *)&server_address,
             sizeof(server_address)) != message_len) {
    fprintf(stderr, "sendto error: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  close(sockfd);
  return EXIT_SUCCESS;
}

