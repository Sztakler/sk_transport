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

int main(int argc, char *argv[]) {

  if (argc < 5) {
    fprintf(stderr, "Invalid number of arguments (4 required, given %d)\n",
            argc - 1);
    return EXIT_FAILURE;
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
    return EXIT_FAILURE;
  } else if (result == -1) {
    perror("<inet_pton>");
    return EXIT_FAILURE;
  }


  if (port < 1024 || port > 65535) {
    fprintf(stderr, "Ranger for ephemeral ports should be 1024-65535 (entered %d)\n", port);
    return EXIT_FAILURE;
  }

  if (size > 10000000) {
    fprintf(stderr, "Size is too large. Enter number between 0-10000000 bytes (entered %d).\n", size);
    return EXIT_FAILURE;
  }

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
