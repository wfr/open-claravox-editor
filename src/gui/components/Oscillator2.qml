// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.15


Item {
    width: childrenRect.width
    height: childrenRect.height

    property alias type: modeSelector.value
    property alias beat: sliderBeatFreq.value
    property alias level: sliderLevel.value
    property alias noiseLevel: sliderNoiseLevel.value

    GridLayout {

        OscillatorMode {
            id: modeSelector
            title: "Osc 2 Mode"
        }

        LabeledSlider {
            id: sliderBeatFreq
            unit: "Hz"
            value: 0.5
            function renderValue() {
                return Number(_interpolate(value, from, to, -10, 10)).toFixed(2);
            }
            description: "Beat Freq"
        }


        LabeledSlider {
            id: sliderLevel
            Layout.columnSpan: 1
            value: 0.5
            unit: "%"
            function renderValue() {
                return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
            }
            description: "Osc 2 Level"
        }

        LabeledSlider {
            id: sliderNoiseLevel
            value: 0.5
            unit: "%"
            function renderValue() {
                return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
            }
            description: "Noise Level"
        }

        rows: 2
        columnSpacing: 34
        columns: 2
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
