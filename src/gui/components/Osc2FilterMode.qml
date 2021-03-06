// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.0

Item {
    id: root

    width: 224
    height: 87

    // Two-way bindings caused trouble, so I opted for a less elegant but working solution:
    // The value is never assigned from the inside:
    property real value: 0.0
    // Instead, a signal is emitted, when it's supposed to be changed in the back-end.
    signal valueSelected(real v)

    QtObject {
        id: internal
        property int mode: 0
    }

    Connections {
        target: root
        function onValueChanged() {
            internal.mode = Math.round(value * 4.0)
        }
    }

    Connections {
        target: internal
        function onModeChanged() {
            repeater.itemAt(internal.mode).checked = true;
            valueSelected(internal.mode / 4.0)
        }
    }

    Component.onCompleted: {
        repeater.itemAt(internal.mode).checked = true;
    }

    ButtonGroup {
        id: modeButtonGroup
    }

    ColumnLayout {
        Text {
            id: text9
            text: "Filter Mode"
            font.pixelSize: 16
            Layout.preferredWidth: 112
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignBottom
            Layout.preferredHeight: 31
            color: Material.foreground
        }

        RowLayout {
            spacing: 0

            Repeater {
                id: repeater
                model: ["LP", "HP", "BP", "BR"]
                Button {
                    text: modelData
                    font.pointSize: 12
                    font.bold: false
                    autoExclusive: true
                    checkable: true
                    checked: false
                    Layout.preferredWidth: 56
                    Layout.preferredHeight: 56
                    autoRepeat: false
                    flat: false
                    ButtonGroup.group: modeButtonGroup
                    onClicked: {
                        internal.mode = index
                    }
                }
            }
        }
    }
}
