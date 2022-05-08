#include "MIDIMessage.h"
#include <exception>
#include <algorithm>
#include <sstream>

MIDIMessage::MIDIMessage(int status, int data1, int data2) {
    if (status < 0 || status > 255
        || data1 < 0 || data1 > 255
        || data2 < 0 || data2 > 255) {
        throw std::runtime_error("invalid MIDI message (value out of bounds)");
    }
    bytes = {uint8_t(status), uint8_t(data1), uint8_t(data2)};
}

MIDIMessage::MIDIMessage(const std::vector<uint8_t>& raw) {
    if (raw.size() < 2 || raw.size() > 3) {
        throw std::runtime_error("invalid MIDI message (unexpected size)");
    }
    this->bytes = raw;
}

bool MIDIMessage::isCC() const {
    return bytes[0] >= 176 && bytes[0] < 192;
}

uint8_t MIDIMessage::channel() const {
    if (bytes[0] < 240)
        return bytes[0] & 0xF;
    else
        return 0;
}

std::string MIDIMessage::str() const {
    std::ostringstream os;
    os << "[" << int(bytes[0]) << "," << int(bytes[1]) << "," << int(bytes[2]) << "]";
    return os.str();
}
