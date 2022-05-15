// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import Qt.labs.qmlmodels 1.0
//import QtQuick.Dialogs 1.3
import Qt.labs.platform

import "components"

import Claravox.Backend 1.0

ColumnLayout {
    x: 0
    y: 0
    //        anchors.fill: parent

    SectionLabel {
        text: qsTr("MIDI IN/OUT SETTINGS")
    }

    GridLayout {
        x: 0
        y: 0
        rows: 2
        columns: 2

        ColumnLayout {
            width: childrenRect.width
            height: childrenRect.height

            Label {
                id: label
                text: qsTr("MIDI Channel In")
            }

            MidiChannel {
                id: midiChannelIn
                omni: true
                channel: Backend.midiChannelIn
                onChannelChanged: Backend.midiChannelIn = channel
            }
        }

        ColumnLayout {
            width: childrenRect.width
            height: childrenRect.height

            Label {
                id: label1
                text: qsTr("MIDI Channel Out")
            }

            MidiChannel {
                id: midiChannelOut
                omni: false
                channel: Backend.midiChannelOut
                onChannelChanged: Backend.midiChannelOut = channel
            }
        }

        ColumnLayout {
            Label {
                id: label2
                text: qsTr("MIDI Output Resolution")
            }

            RowLayout {
                spacing: 0

                Button {
                    id: button7bit
                    text: qsTr("7-bit")
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 40
                    Layout.fillWidth: true
                    flat: false
                    onClicked: {
                    }
                }

                Button {
                    id: button14bit
                    text: qsTr("14-bit")
                    autoExclusive: true
                    checkable: true
                    checked: true
                    Layout.preferredWidth: 40
                    Layout.fillWidth: true
                    flat: false
                    onClicked: {
                    }
                }
            }
        }
    }

    SectionLabel {
        text: qsTr("MIDI CONTROLLER SETTINGS (PER PRESET)")
    }
    GridLayout {
        id: gridLayout
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        columns: 2

        ColumnLayout {
            Label {
                text: "Note Number Mode"
            }
            RowLayout {
                spacing: 0

                Button {
                    id: buttonNoteNumberFixed
                    text: qsTr("Fixed")
                    autoExclusive: true
                    checkable: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 40
                    flat: false
                    onClicked: {
                    }
                }

                Button {
                    id: buttonNoteNumberChromatic
                    text: qsTr("Chromatic")
                    autoExclusive: true
                    checkable: true
                    checked: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 40
                    flat: false
                    onClicked: {
                    }
                }
            }
        }

        LabeledSlider {
            id: noteNumber
            value: 80
            unit: ""
            to: 127
            description: "Note     Number"
        }

        ColumnLayout {
            Label {
                text: "MIDI Pitch Out"
            }
            RowLayout {
                spacing: 0

                Button {
                    id: buttonMidiPitchOutOff
                    text: qsTr("O")
                    autoExclusive: true
                    checkable: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 40
                    flat: false
                    onClicked: {
                    }
                }

                Button {
                    id: buttonMidiPitchOutOn
                    text: qsTr("I")
                    autoExclusive: true
                    checkable: true
                    checked: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 40
                    flat: false
                    onClicked: {
                    }
                }
            }
        }

        LabeledSlider {
            id: pitchBendRange
            description: "Pitchbend Range"
            unit: ""
            value: 48
            to: 96
        }
    }
} // MIDI
