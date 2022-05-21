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
import Qt.labs.platform 1.1

import "components"

import Claravox.Backend 1.0

Item {
    id: item1
    width: 800
    height: 500
    ColumnLayout {
        width: parent.width
        Text {
            text: "Open Claravox Editor"
            font.pixelSize: 36
            Layout.bottomMargin: 16
            Layout.topMargin: 16
            font.bold: true
        }
        GridLayout {
            id: gridLayout
            width: 800
            columnSpacing: 32
            rowSpacing: 16
            columns: 2
            ColumnLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                SectionLabel { text: "App Information" ;}
                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "App Version:" }
                    Text { text: Qt.application.version; horizontalAlignment: Text.AlignRight; Layout.fillWidth: true; }
                }
            }
            ColumnLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
                SectionLabel { text: "Instrument Information" }
                RowLayout {
                    Text { text: "Firmware Version:" }
                    Text { text: "N/A";horizontalAlignment: Text.AlignRight; Layout.fillWidth: true; }
                }
                RowLayout {
                    Text { text: "Serial No:" }
                    Text { text: "N/A";horizontalAlignment: Text.AlignRight; Layout.fillWidth: true; }
                }
            }
            Button {
                text: "Open User Manual" ;
                enabled: false;
                Layout.fillWidth: true
            }
            Button {
                text: "Update Firmware" ;
                enabled: false;
                Layout.fillWidth: true
            }
            Text {
                text: "What's new:"
            }
            Text {
                text: "What's new:"
            }
            Flickable {
                height: 120
                // QTBUG-56075 mediocre mitigation:
//                maximumFlickVelocity: 500
                Layout.fillWidth: true
                flickableDirection: Flickable.VerticalFlick
                boundsMovement: Flickable.StopAtBounds
                boundsBehavior: Flickable.DragOverBounds
                clip: true
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
                TextArea.flickable: TextArea {
                    readOnly: true
                    wrapMode: Text.WordWrap
                    topPadding: 0
                    bottomPadding: 0
                    rightPadding: 24
                    background: null
                    text: Backend.appChangelog()
                    textFormat: Text.MarkdownText
                }
            }
            Flickable {
                height: 120
                // QTBUG-56075 mediocre mitigation:
//                maximumFlickVelocity: 500
                Layout.fillWidth: true
                flickableDirection: Flickable.VerticalFlick
                boundsMovement: Flickable.StopAtBounds
                boundsBehavior: Flickable.DragOverBounds
                clip: true
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
                TextArea.flickable: TextArea {
                    readOnly: true
                    wrapMode: Text.WordWrap
                    topPadding: 0
                    bottomPadding: 0
                    rightPadding: 24
                    background: null
                    textFormat: Text.MarkdownText
                    text: "### 1.0.2
- Properly use MSB and LSB for interpreting RPN 0 Pitch Bend Sensitivity.
- Express instrument MIDI input and output channels as absolute numbers instead
  of normalized across the full 14-bit resolution of NRPN 1026 and 1027.
- Pitch Antenna Mod to Wavescan Freq parameter now properly clamps to 0.1Hz and 200Hz instead of rolling over.
- Fixed MIDI Pitch Mode now uses the full 0-16383 range of bend values and
  sends RPN 0 Pitch Bend Sensitivity before any note on event.

### 1.0.1
- Initial Release"
                }
            }
            Button {
                text: "View App History";
                Layout.fillWidth: true
                enabled: false;
            }
            Button {
                text: "View Firmware History" ;
                Layout.fillWidth: true
                enabled: false;
            }
            Button {
                text: "View App Source Code on GitHub";
                Layout.fillWidth: true
                onClicked: {
                    Qt.openUrlExternally("https://github.com/wfr/open-claravox-editor");
                }
            }
            Pane {

            }
            Text {
                text: "Open Claravox Editor\n© 2022 Wolfgang Frisch"
                font.pixelSize: 14
            }
            Text {
                text: "Firmware and presets\n© Moog Inc."
                font.pixelSize: 14
            }
        }
    }
}
