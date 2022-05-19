#include "packet_data.h"
#include <vector>

PacketData::PacketData(int size, int start_byte) {
    received = false;
    start_byte = start_byte;
    bytes = std::vector<uint8_t>(size, 0);
}