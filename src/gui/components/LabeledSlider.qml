// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

/* Labeled slider with value/unit displays
   which can be customized by overriding renderValue() and renderUnits().

   NOTE: Slider.onValueChanged() fires about 100 times a second.
   It might make sense to implement rate-limiting.
*/
Item {
    id: item1
    width: 224
    height: 87
    property alias description: text_description.text
    property alias value: slider.value
    property alias from: slider.from
    property alias to: slider.to
    property alias stepSize: slider.stepSize
    property int decimals: 0
    property string unit: "kHz"

    // override this to display custom values
    function renderValue() {
        return value.toFixed(decimals);
    }

    // override this to display custom units
    function renderUnit() {
        return unit;
    }

    // map [src_a ... src_b] --> [dst_a ... dst_b]
    function _interpolate(x, src_a, src_b, dst_a, dst_b) {
        return dst_a + ((dst_b - dst_a) / (src_b - src_a)) * (x - src_a)
    }

    // only necessary for discrete parameters, e.g. root note
    function normalize() {
        return _interpolate(value, from, to, 0.0, 1.0)
    }

    function denormalize(src) {
        return _interpolate(src, 0.0, 1.0, from, to)
    }

//    Layout.topMargin: 8
//    Layout.bottomMargin: 8

// DEBUG:
//    Rectangle {
//        width: parent.width
//        height: parent.height
//        border.color: "#000"
//        color: "transparent"
//    }

    ColumnLayout {
        x: 0
        anchors.fill: parent
        spacing: 0
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: text_description
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignBottom
                Layout.preferredWidth: 80
                textFormat: Text.PlainText
//                color: Material.foreground
                text: "Description"
                font.pixelSize: 16
            }

            Item {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: true
            }

            RowLayout {
//                Layout.preferredWidth: 128
                spacing: 2
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                Text {
                    id: text_value
                    text: renderValue()
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignBottom
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignBaseline
                    textFormat: Text.PlainText
                    color: Material.foreground
                }

                Text {
                    id: text_unit
                    text: renderUnit()
                    elide: Text.ElideRight
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignBottom
                    Layout.alignment: Qt.AlignRight | Qt.AlignBaseline
                    textFormat: Text.PlainText
                    color: Material.foreground
                }
            }
        }

        Slider {
            id: slider
            Layout.preferredHeight: 40
//            Layout.preferredWidth: 228
            Layout.fillWidth: true
            Layout.columnSpan: 2
            from: 0
            to: 1.0
            value: 0.5
            stepSize: 0.0
        }
    }
}



/*##^##
Designer {
    D{i:0;height:87;width:240}D{i:2}
}
##^##*/
