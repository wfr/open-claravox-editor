// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.0

/* TODO: replace hardcoded buttons with a Model and a Repeater */

Item {
    id: root

    property string title: "Osc Mode"
    property real value: 0.0

    width: childrenRect.width
    height: childrenRect.height

    Connections {
        target: root
        function onValueChanged() {
            let index = value * 4;
            switch(index) {
            case 0:
                buttonSine.checked = true;
                break;
            case 1:
                buttonTriangle.checked = true;
                break;
            case 2:
                buttonSaw.checked = true;
                break;
            case 3:
                buttonWavetable.checked = true;
                break;
            }
        }
    }


    ColumnLayout {
        Text {
            id: textTitle
            text: title
            font.pixelSize: 16
            verticalAlignment: Text.AlignBottom
            Layout.preferredWidth: 112
            Layout.preferredHeight: 25
            textFormat: Text.PlainText
            color: Material.foreground
        }

        RowLayout {
            Button {
                id: buttonSine
                text: qsTr("")
                Layout.preferredWidth: 56
                Layout.preferredHeight: 56
                autoExclusive: true
                checkable: true
                checked: true
                font.pointSize: 12
                autoRepeat: false
                icon.name: "Sine"
                icon.source: "../icons/sine.png"
                onPressed: {
                    value = 0.0;
                }
            }

            Button {
                id: buttonTriangle
                text: qsTr("")
                Layout.preferredWidth: 56
                Layout.preferredHeight: 56
                flat: false
                autoExclusive: true
                checkable: true
                font.pointSize: 12
                autoRepeat: false
                checked: false
                icon.name: "Triangle"
                icon.source: "../icons/triangle.png"
                onPressed: {
                    value = 0.25;
                }
            }

            Button {
                id: buttonSaw
                text: ""
                Layout.preferredWidth: 56
                Layout.preferredHeight: 56
                flat: false
                autoExclusive: true
                checkable: true
                font.pointSize: 12
                autoRepeat: false
                icon.name: "Sawtooth"
                icon.source: "../icons/sawtooth.png"
                onPressed: {
                    value = 0.5;
                }
            }

            Button {
                id: buttonWavetable
                text: ""
                Layout.preferredWidth: 56
                Layout.preferredHeight: 56
                flat: false
                autoExclusive: true
                checkable: true
                font.pointSize: 12
                autoRepeat: false
                icon.name: "Wavetable"
                icon.source: "../icons/wavetable.png"
                onPressed: {
                    value = 0.75;
                }
            }
            spacing: 0
        }
    }
}
