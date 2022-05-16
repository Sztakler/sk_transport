#pragma once

#include <netinet/in.h>
#include <string>

class Transport {
public:
  struct in_addr ipv4_address;
  std::string filename;
  uint port;
  uint size;

  int sockfd;

public:
  Transport(int argc, char* argv[]);

public:
  void initialize_socket();

private:
  void create_socket();
  void bind_socket_to_port();
};

