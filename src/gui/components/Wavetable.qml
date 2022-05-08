// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.0

import Claravox.BackEnd 1.0

Item {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    property alias scanPos: sliderScanPos.value
    property alias scanFreq: sliderScanFreq.value
    property alias scanAmount: sliderScanAmount.value

    // Two-way bindings caused trouble, so I opted for a less elegant but working solution:
    // The value is never assigned from the inside:
    property real wavetable: 0.0
    // Instead, a signal is emitted, when it's supposed to be changed in the back-end.
    signal wavetableSelected(real v)

    QtObject {
        id: internal
        property int wtIndex: 0
    }

    Connections {
        target: root
        function onWavetableChanged() {
            let index = wavetable * 8.0;
            if (index < 0 || index > 8) {
                console.log("[ERROR] Wavetable.onWavetableChanged bad index", index);
            }
            internal.wtIndex = index
        }
    }

    Connections {
        target: internal
        function onWtIndexChanged() {
            buttonRepeater.itemAt(internal.wtIndex).checked = true;
            wavetableSelected(internal.wtIndex / 8.0)
        }
    }

    ButtonGroup {
        id: wavetableButtonGroup
    }

    GridLayout {
        columns: 2
        rows: 2
        columnSpacing: 34  // TODO: parametrize (issue #8)

        GridLayout {
            id: buttonGrid
            columns: 4
            rows: 2
            columnSpacing: 0
            rowSpacing: 0
            Repeater {
                id: buttonRepeater
                model: [1,2,3,4,5,6,7,8]
                delegate: Button {
                    text: modelData
                    font.pointSize: 12
                    font.bold: false
                    checkable: true
                    bottomInset: 0
                    topInset: 0
                    Layout.preferredWidth: 54
                    Layout.preferredHeight: 40
                    autoRepeat: false
                    flat: false
                    ButtonGroup.group: wavetableButtonGroup
                    onClicked: {
                        internal.wtIndex = modelData - 1
                    }
                }
            }
        }

        LabeledSlider {
            id: sliderScanPos
            Layout.columnSpan: 1
            value: 0.5
            unit: "%"
            description: "Scan Position"
            function renderValue() {
                return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
            }
        }

        LabeledSlider {
            id: sliderScanFreq
            Layout.columnSpan: 1
            value: 0.5
            unit: "Hz"
            description: "Scan Freq"
            function renderValue() {
                let hz = BackEnd.params.oscScanFreqToHz(value);
                if (hz < 100) {
                    return Number(hz).toFixed(1);
                } else {
                    return Number(Math.ceil(hz)).toFixed(0);
                }
            }
        }

        LabeledSlider {
            id: sliderScanAmount
            Layout.columnSpan: 1
            value: 0.5
            unit: "%"
            description: "Scan Amount"
            function renderValue() {
                return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
            }
        }
    }

}
