#include "packet_data.h"
#include <vector>

PacketData::PacketData(int size, int start_byte) {
    this->received = false;
    this->start_byte = start_byte;
    this->size = size;
    this->bytes = std::vector<uint8_t>(size, 0);
}