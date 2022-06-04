// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <vector>
#include <cstdint>
#include <string>

class MIDIMessage {
public:
    std::vector<uint8_t> bytes;

    MIDIMessage(int status, int data1, int data2);
    MIDIMessage(const std::vector<uint8_t>& raw);

    bool isCC() const;
    uint8_t channel() const;
    std::string str() const;
};
