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

  while (true) {
    transport.send_requests();
    transport.listen_for_responses();

    if (transport.saved_bytes >= transport.size) {
      std::cout << "SAVED " << transport.saved_bytes << "/" << transport.size
                << " BYTES TO FILE\n";
      break;
    }
  }
  // fclose(transport.fp);
  close(transport.file_descriptor);

  return 0;
}
