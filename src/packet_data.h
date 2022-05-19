#pragma once

#include <cstdint>
#include <sys/types.h>
#include <vector>

class PacketData {
public:
  std::vector<uint8_t> bytes;
  bool received;

  uint start_byte;
  uint size;
  
  public:
  PacketData(int size, int start_byte);
};