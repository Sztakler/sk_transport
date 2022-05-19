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

  // int retVal;
  // FILE *fp;
  // char buffer[] = "KURWAAAA.";

  // fp = fopen("myfile.txt", "w");

  // retVal = fwrite(buffer, sizeof(char), strlen(buffer), fp);
  // fclose(fp);
  // std::cout << "When count = 0, fwrite returned " << retVal << std::endl;

  // retVal = fwrite(buffer,0,1,fp);
  // std::cout << "When size = 0, fwrite returned " << retVal << std::endl;

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
  fclose(transport.fp);

  return 0;
}
