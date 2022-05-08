// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch

// References:
// - https://www.midimountain.com/midi/midi_status.htm
// - https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes
//
// TODO:
// - Handle rtmidi errors.
//   https://www.music.mcgill.ca/~gary/rtmidi/classRtMidiError.html

#include "MIDIMessage.h"
#include "Claravox.h"
#include "RtMidi.h"
#include <exception>
#include <chrono>
#include <thread>
#include <QTimer>
#include <QMap>
#include <set>

std::vector<MIDIMessage> midiMessagesFromDouble(const MIDIParameter& param, double val, uint8_t channel) {
    std::vector<MIDIMessage> result;
    assert(channel >= 0 && channel <= 16);
    if (param.max_value <= 127) {
        MIDIMessage msg(176 + channel, param.cc, val * param.max_value);
        result.push_back(msg);
    } else {
        uint16_t word = uint16_t(val * param.max_value);
        uint8_t msb = word >> 7;
        uint8_t lsb = word & 127;
        MIDIMessage msg1(176 + channel, param.cc, msb);
        MIDIMessage msg2(176 + channel, param.cc | 32 /* LSB bit */, lsb);
        result.push_back(msg1);
        result.push_back(msg2);
    }
    return result;
}


Claravox::Claravox(QObject *parent, unsigned app_channel)
    : QObject(parent),
      midiin(nullptr),
      midiout(nullptr),
      m_app_channel(app_channel)
{

    try {
        midiin = new RtMidiIn();
        midiout = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        throw std::runtime_error("RtMidi initialization failed.");
    }
}

void Claravox::start() {
    std::cout << "Waiting for Claravox MIDI interface to show up..." << std::endl;
    QTimer* detection_loop = new QTimer(this);
    detection_loop->setSingleShot(false);
    connect(detection_loop, &QTimer::timeout, [this]() {
        this->detect();
    });
    detection_loop->start(250);
}

void Claravox::detect() {
    if (!midiin->isPortOpen()) {
        inport = NOT_FOUND;
        unsigned int nPorts = midiin->getPortCount();
//        std::cout << "Waiting for Claravox input port. " << nPorts << " MIDI input sources available." << std::endl;
        std::string portName;
        for (unsigned int i=0; i<nPorts; i++) {
            try {
                portName = midiin->getPortName(i);
            }
            catch ( RtMidiError &error ) {
                error.printMessage();
            }
            if (portName.find("Moog Claravox:") != std::string::npos) {
                inport = i;
                std::cout << "  Input Port #" << i+1 << ": " << portName << "\n";
            }
        }
        if (inport != NOT_FOUND) {
            midiin->openPort(inport);
            midiin->ignoreTypes(false, false, false);
            midiin->setCallback(&Claravox::rtmidi_callback_, static_cast<void*>(this));

            emit connected(); // TODO: emit only if both ports are connected
        }
    }

    // Check outputs.
    if (!midiout->isPortOpen()) {
        outport = NOT_FOUND;
        unsigned int nPorts = midiout->getPortCount();
//        std::cout << "Waiting for Claravox output port. " << nPorts << " MIDI output ports available." << std::endl;
        std::string portName;
        for ( unsigned int i=0; i<nPorts; i++ ) {
            try {
                portName = midiout->getPortName(i);
            }
            catch (RtMidiError &error) {
                error.printMessage();
            }
            if (portName.find("Moog Claravox") != std::string::npos) {
                outport = i;
                std::cout << "  Output Port #" << i+1 << ": " << portName << " <--\n";
            }
        }
        if (outport != NOT_FOUND) {
            midiout->openPort(outport);
            sendFirstContact();
        }
    }

}

void Claravox::rtmidi_callback(double timestamp, std::vector<unsigned char> bytes) {
    MIDIMessage msg(bytes);
    if (msg.isCC()) {
        remote_state[bytes[1]] = bytes[2];
    }
//    std::cout << "IN:  " << msg.str() << std::endl;

    // Debug: parse MIDI messages
    std::cout << "IN:  " << msg.str() << " ";
    if (msg.isCC()) {
        int cc = msg.bytes[1] & ~32;
        auto mp = mpFromCC(cc);
        if (mp.has_value()) {
            std::cout << mp->name;
        } else if (msg.bytes[1] == 99) {
            std::cout << "NRPN LSB";
        } else if (msg.bytes[1] == 98) {
            std::cout << "NRPN MSB";
        } else if (msg.bytes[1] == 6) {
            std::cout << "Data Entry MSB";
        } else if (msg.bytes[1] == 38) {
            std::cout << "Data Entry LSB";
        } else {
            std::cout << "UNKNOWN CC";
        }
    } else if (msg.bytes[0] >= 192 && msg.bytes[0] <= 207) {
        // Program Change
//        int channel = (msg.bytes[0] - 192) + 1;
        int program = msg.bytes[1];
        std::cout << "Program Change " << program;
    } else {
        std::cout << "UNKNOWN MESSAGE";
    }
    std::cout << std::endl;


    emit messageReceived(timestamp, msg);
}

void Claravox::rtmidi_callback_(double timestamp, std::vector<unsigned char> *message_vector, void *selfptr) {
    reinterpret_cast<Claravox*>(selfptr)->rtmidi_callback(timestamp, *message_vector);
}

void Claravox::send(const MIDIMessage& msg) {
    if (msg.isCC()) {
        unsigned char data1 = msg.bytes[1];
        unsigned char data2 = msg.bytes.at(2);
        if (remote_state.count(data1)) {
            if (remote_state[data1] == data2) {
                return;
            } else {
                remote_state.erase(data1);
            }
        }
    }

    std::cout << "OUT: " << msg.str() << std::endl;

    if (midiout && midiout->isPortOpen()) {
        midiout->sendMessage(&msg.bytes);
    }
}

void Claravox::sendNRPN(int channel, uint16_t nrpn, uint16_t data) {
    std::vector<MIDIMessage> messages;
    messages.push_back({uint8_t(0xb0 + channel), 0x62, uint8_t(nrpn & 127)}); // NRPN LSB
    messages.push_back({uint8_t(0xb0 + channel), 0x63, uint8_t(nrpn >> 7)});  // NRPN MSB
    messages.push_back({uint8_t(0xb0 + channel), 0x06, uint8_t(nrpn >> 7)});  // Data Entry (MSB)
    messages.push_back({uint8_t(0xb0 + channel), 0x26, uint8_t(nrpn & 127)}); // Data Entry (LSB)
    std::cout << "--> NRPN (" << nrpn << ", " << data << "), packets below:" << std::endl;
    for (auto& msg : messages) {
        this->send(msg);
    }
}

void Claravox::setMidiResolution(bool high) {
    sendNRPN(m_app_channel, 1025, high ? 8192 : 0);
}

void Claravox::setMidiChannelIn(int channel_in) { // 0 = Omni
    for (uint8_t c = 0; c < 16; c++) {
        sendNRPN(c, 1026, channel_in);
    }
}

void Claravox::setMidiChannelOut(int channel_out) {
    sendNRPN(m_app_channel, 1027, channel_out);
}

void Claravox::sendFirstContact() {
    std::cout << "Claravox first contact." << std::endl;
    std::vector<unsigned char> msg({0xF0, 0xF7});
    midiout->sendMessage(&msg);

     // TODO make configurable
    m_channel_in = 0;
    m_channel_out = 1;

    std::cout << "Broadcast MIDI Channel In = " << m_channel_in << " on all channels" << std::endl;
    setMidiChannelIn(m_channel_in);
    std::cout << "Set MIDI Channel Out = " << m_channel_out << std::endl;
    setMidiChannelOut(m_channel_out);

    // The following gets a ton of responses, yet-to-be deciphered.
    // (0x76, 0x7f) and (0x76, 0x7e) seem to be start/end markers?
    // When the user configures the CV mod matrix,
    // the same markers are surrounding a ton of messages.
    std::cout << "Send mystery init messages" << std::endl;
    std::vector<std::vector<uint8_t>> hello3 = {
        {uint8_t(0xb0 + m_app_channel), 0x76, 0x7f},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x00},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x00},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x1c},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x00},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x00},
        {uint8_t(0xb0 + m_app_channel), 0x75, 0x00},
        {uint8_t(0xb0 + m_app_channel), 0x76, 0x7e},
    };
    for (auto& bytes : hello3) {
        midiout->sendMessage(&bytes);
    }

    std::cout << "NRPN 1011, 1012, 2383, 284 = 0" << std::endl;
    sendNRPN(m_app_channel, 1011, 0); // Unknown
    sendNRPN(m_app_channel, 1012, 0); // Unknown
    sendNRPN(m_app_channel, 2383, 0); // Unknown
    sendNRPN(m_app_channel, 2384, 0); // Unknown

    std::cout << "Set MIDI Channel In = " << m_channel_in << std::endl;
    sendNRPN(m_app_channel, 1026, m_channel_in);

    // Moog's app only sends the above ^
    // Everything below deviates from Moog's app.
    std::cout << "Set MIDI Resolution to 14 bits" << std::endl;
    setMidiResolution(true); // we don't have good 7-bit support yet
}
