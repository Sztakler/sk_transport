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
  transport.send_requests();
  transport.listen_for_responses();

  return 0;
}
