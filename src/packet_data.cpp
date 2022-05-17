#include "packet_data.h"
#include <vector>

PacketData::PacketData(int size) {
    received = false;
    bytes = std::vector<uint8_t>(size, 0);
}