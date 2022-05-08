// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch

// This file is a development sandbox, not intended for actual use.

#include <QCoreApplication>
#include <iostream>
#include "Claravox.h"
#include <MIDIParameter.h>

/* Primitive NRPN parser, hardcoded to the Claravox.
 * Expects, in that order:
 * - NRPN LSB (99)
 * - NRPN MSB (98)
 * - Data Entry MSB (6)
 * - Data Entry LSB (38)
 */
class NRPNParser {
public:
    bool feed(const MIDIMessage& msg) {
        if (!msg.isCC()) {
            return false;
        }
        switch (buffer.size()) {
        case 0:
            if (msg.bytes[1] == 99) {
                buffer.push_back(msg);
            }
            return false;
        case 1:
            if (msg.bytes[1] == 98) {
                buffer.push_back(msg);
            } else {
                buffer.clear();
            }
            return false;
        case 2:
            if (msg.bytes[1] == 6) {
                buffer.push_back(msg);
            } else {
                buffer.clear();
            }
            return false;
        case 3:
            if (msg.bytes[1] == 38) {
                buffer.push_back(msg);
                m_nrpn = buffer[0].bytes[2] + (buffer[1].bytes[2] << 7);
                m_data = (buffer[2].bytes[2] << 7) + buffer[3].bytes[2];
            }
            buffer.clear();
            return true;
        }
        return false; // unreachable
    }

    uint16_t nrpn() {
        return m_nrpn;
    }

    uint16_t data() {
        return m_data;
    }


protected:
    std::vector<MIDIMessage> buffer;
    uint16_t m_nrpn, m_data;
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TEMP: print MIDI parameters as .rst
    for (const MIDIParameter& mp : midiparameters) {
        std::cout << "   * - " << mp.name << "\n";
        std::cout << "     - " << int(mp.cc) << "\n";
        std::cout << "     - " << mp.nrpn << "\n";
        std::cout << "     - 0 - " << mp.max_value << "\n";
        std::cout << "     - | " << mp.description << "\n";
    }
    std::cout << std::endl;

    static NRPNParser nrpn_parser;

    Claravox claravox;
    QObject::connect(&claravox, &Claravox::connected, []() {
        std::cout << "Claravox connected." << std::endl;
    });
    QObject::connect(&claravox, &Claravox::disconnected, []() {
        std::cout << "Claravox disconnected." << std::endl;
    });
    QObject::connect(&claravox, &Claravox::messageReceived, [](double timestamp, MIDIMessage message) {
//        std::cout << "MIDI IN:  " << message.str() << std::endl;
        /* Example NRPN input:
        IN:  [176,99,7] UNKNOWN CC
        IN:  [176,98,115] UNKNOWN CC
        IN:  [176,6,0] UNKNOWN CC
        IN:  [176,38,0] UNKNOWN CC
        */
        if (nrpn_parser.feed(message)) {
            std::cout << "<-- NRPN " << nrpn_parser.nrpn() << " = " << nrpn_parser.data() << std::endl;
        }
    });
    claravox.start();

    return a.exec();
}
