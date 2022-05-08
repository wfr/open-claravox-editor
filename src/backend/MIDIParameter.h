#pragma once

#include <optional>
#include <cstdint>
#include <QString>

class MIDIParameter {
public:
    const char* name;
    unsigned char cc;
    unsigned short nrpn;
    unsigned short max_value;
    const char* description;
};

extern const std::vector<MIDIParameter> midiparameters;

std::optional<const MIDIParameter> mpFromCC(uint8_t cc);
std::optional<const MIDIParameter> mpFromName(const QString& name);
