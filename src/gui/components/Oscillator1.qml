// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.15


Item {
    width: childrenRect.width
    height: childrenRect.height

    property alias type: modeSelector.value
    property alias level: sliderLevel.value

    GridLayout {

        OscillatorMode {
            id: modeSelector
            title: "Osc 1 Mode"
        }

        LabeledSlider {
            id: sliderLevel
            Layout.columnSpan: 1
            value: 0.5
            unit: "%"
            description: "Osc 1 Level"
            function renderValue() {
                return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
            }
        }

        rows: 2
        columnSpacing: 34
        columns: 1
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
