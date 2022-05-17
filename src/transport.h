#pragma once

#include <netinet/in.h>
#include <netinet/ip.h>
#include <string>
#include <sys/types.h>
#include <vector>

#include "packet_data.h"

class Transport {
public:
  struct in_addr ipv4_address;
  std::string filename;
  uint port;
  uint size;
  
  uint head;
  uint tail;

  int sockfd;

  const uint DATAGRAMS_IN_WINDOW = 100;
  const uint BYTES_IN_DATAGRAM = 10;

  std::vector<PacketData> window; 

public:
  Transport(int argc, char* argv[]);

public:
  void initialize_socket();
  void send_requests();
  void listen_for_responses();

  void print_window();

private:
  void create_socket();
  void bind_socket_to_port();
  void send_datagram(int start, int end);
  void read_socket();
  void store_received_data(char buffer[IP_MAXPACKET + 1], ssize_t received_bytes);
  int get_datagram_index(uint datagram_start, uint datagram_end);
};

