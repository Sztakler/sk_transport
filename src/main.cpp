/*
Imię i nazwisko: Krystian Jasionek
Numer indeksu:   317806
*/

#include "transport.h"
#include <iostream>

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

  transport.close_file();

  return 0;
}
