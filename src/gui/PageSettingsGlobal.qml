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

// Global
GridLayout {
    RowLayout {
        ColumnLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            SectionLabel {
                text: "CONTROL VOLTAGE OUT"
            }
            LabeledSlider {
                id: sliderVolumeCvOutScale
                description: "Volume CV Out Scale"
            }
            Label {
                text: "Volume CV Out Range"
            }
            RowLayout {
                spacing: 0
                Button {
                    id: buttonVolumeCvOutPlusMinus5V
                    text: "±5v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
                Button {
                    id: buttonVolumeCvOut0To5V
                    text: "0-5v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
                Button {
                    id: buttonVolumeCvOut0To10V
                    text: "0-10v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
            }

            Label {
                text: "Pitch CV Out Range"
            }
            RowLayout {
                spacing: 0
                Button {
                    id: buttonPitchCvOutPlusMinus5V
                    text: "±5v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
                Button {
                    id: buttonPitchCvOut0To5V
                    text: "0-5v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
                Button {
                    id: buttonPitchCvOut0To10V
                    text: "0-10v"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
            }

            Label {
                text: "Pitch CV Out Quantize"
            }
            RowLayout {
                spacing: 0
                Button {
                    id: buttonPitchCvQuantizeOff
                    text: "O"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
                Button {
                    id: buttonPitchCvQuantizeOn
                    text: "I"
                    autoExclusive: true
                    checkable: true
                    Layout.preferredWidth: 80
                    flat: false
                    onClicked: {
                    }
                }
            }

            Button {
                text: "Reset CV Out to Defaults"
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            SectionLabel {
                text: "CONTROL VOLTAGE IN"
            }
            LabeledSlider {
                id: sliderVolumeCvInScale
                description: "Volume CV In Scale"
            }
            LabeledSlider {
                id: sliderVolumeCvInOffset
                description: "Volume CV In Offset"
            }
        }
    }
}
