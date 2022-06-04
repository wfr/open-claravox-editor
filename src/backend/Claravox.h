// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once
#include <QObject>
#include <optional>
#include "MIDIMessage.h"
#include "MIDIParameter.h"

class RtMidiIn;
class RtMidiOut;
class MIDIMessage;

std::vector<MIDIMessage> midiMessagesFromDouble(const MIDIParameter& param, double val, uint8_t channel);

/*!
 * \brief This class communicates with the instrument.
 */
class Claravox : public QObject
{
    Q_OBJECT
public:
    explicit Claravox(QObject *parent = nullptr, unsigned app_channel = 1);
    void start();
    void send(const MIDIMessage& msg);
    void sendNRPN(int channel, uint16_t nrpn, uint16_t data);

signals:
    void connected();
    void disconnected();
    void messageReceived(double timestamp, MIDIMessage message);

private:
    RtMidiIn  *midiin = nullptr;
    RtMidiOut *midiout = nullptr;

    const unsigned NOT_FOUND = 0xFFFF;
    unsigned inport = NOT_FOUND;
    unsigned outport = NOT_FOUND;
    unsigned m_app_channel;
    unsigned m_channel_in, m_channel_out;

    // Keep track of the instrument's state.
    //
    // Why? The backend does not discern where parameter changes originate.
    // The backend fires a change event whenever a parameter is changed,
    // be it due to user interaction with the GUI, or after a physical interaction with the instrument.
    // This is not a problem per se, but it causes unnecessary outgoing MIDI spam:
    // Instrument ---> MIDI IN ---> Backend ---> MIDI OUT ---> Instrument.
    // In order to avoid these echoes, we track some of the instrument's state.
    // Maybe not the most elegant solution but it's good enough for now.
    //
    // TODO:
    // - Consider invalidating indiscriminately every few seconds to recover from yet unknown bugs
    std::map<uint8_t, uint8_t> remote_state; // CC -> Value

    void detect();
    void rtmidi_callback(double timestamp, std::vector<unsigned char> bytes);
    static void rtmidi_callback_(double timestamp, std::vector<unsigned char> *bytes, void* selfptr);

    void setMidiResolution(bool high);
    void setMidiChannelIn(int channel_in); // 0 = Omni
    void setMidiChannelOut(int channel_out);
    void sendFirstContact();
};
