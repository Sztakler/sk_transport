#pragma once

#include <netinet/in.h>
#include <netinet/ip.h>
#include <string>
#include <sys/types.h>
#include <vector>
#include <deque>

#include "packet_data.h"

class Transport {
public:
  struct in_addr ipv4_address;
  std::string filename;
  std::string filemode;

  u_int16_t port;
  uint size;
  uint saved_bytes;
  int last_index;
  int file_descriptor;
  
  uint head;
  uint tail;

  int sockfd;

  const uint DATAGRAMS_IN_WINDOW = 16;
  const uint BYTES_IN_DATAGRAM = 16;

  FILE *fp;

  std::deque<PacketData> window; 

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


  uint get_data_start_index(char buffer[IP_MAXPACKET + 1]);
  bool sender_is_valid(struct sockaddr_in sender);

  void write_window_prefix_to_file();
  int calculate_received_datagrams_prefix_length();
  void write_to_file(int i);
  void pop_window_prefix(int i);
  void push_window_suffix(int i);
};

