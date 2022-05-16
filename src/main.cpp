#include "transport.h"
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

  Transport transport(argc, argv);

  transport.initialize_socket();
  transport.send_data();

  // for (;;) {

  //   struct sockaddr_in sender;
  //   socklen_t sender_len = sizeof(sender);
  //   u_int8_t buffer[IP_MAXPACKET + 1];

  //   ssize_t datagram_len = recvfrom(transport.sockfd, buffer, IP_MAXPACKET, 0,
  //                                   (struct sockaddr *)&sender, &sender_len);
  //   if (datagram_len < 0) {
  //     fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
  //     return EXIT_FAILURE;
  //   }

  //   char sender_ip_str[20];
  //   inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str,
  //             sizeof(sender_ip_str));
  //   printf("Received UDP packet from IP address: %s, port: %d\n", sender_ip_str,
  //          ntohs(sender.sin_port));

  //   buffer[datagram_len] = 0;
  //   printf("%ld-byte message: +%s+\n", datagram_len, buffer);

  //   char *reply = "Thank you!";
  //   ssize_t reply_len = strlen(reply);
  //   if (sendto(transport.sockfd, reply, reply_len, 0,
  //              (struct sockaddr *)&sender, sender_len) != reply_len) {
  //     fprintf(stderr, "sendto error: %s\n", strerror(errno));
  //     return EXIT_FAILURE;
  //   }

  //   fflush(stdout);
  // }

  // close(transport.sockfd);
  // return EXIT_SUCCESS;

  // Sender
  // int transport.sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // if (transport.sockfd < 0) {
  //   fprintf(stderr, "socket error: %s\n", strerror(errno));
  //   return EXIT_FAILURE;
  // }

  // struct sockaddr_in server_address;
  // bzero(&server_address, sizeof(server_address));
  // server_address.sin_family = AF_INET;
  // server_address.sin_port = htons(32345);
  // inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

  // char *message = "Hello server!";
  // ssize_t message_len = strlen(message);
  // if (sendto(transport.sockfd, message, message_len, 0,
  //            (struct sockaddr *)&server_address,
  //            sizeof(server_address)) != message_len) {
  //   fprintf(stderr, "sendto error: %s\n", strerror(errno));
  //   return EXIT_FAILURE;
  // }

  // close(transport.sockfd);
  // return EXIT_SUCCESS;
}
