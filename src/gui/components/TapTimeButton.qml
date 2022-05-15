// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.0

Button {
    id: buttonTapTempo

    property int numSamples: 8
    property int maxDelta: 2000

    signal measured(real delta)

    function _interpolate(x, src_a, src_b, dst_a, dst_b) {
        return dst_a + ((dst_b - dst_a) / (src_b - src_a)) * (x - src_a)
    }

    QtObject {
        id: internal
        property real last_time: 0;
        property var deltas: []
    }

    Connections {
        target: buttonTapTempo
        function onPressed() {
            let now = new Date().getTime();
            let delta = now - internal.last_time;
            internal.last_time = now;

            if (delta < maxDelta) {
                internal.deltas.push(delta);
            } else {
                // Discard history after maxDelta has passed.
                internal.deltas.length = 0;
            }

            while (internal.deltas.length > numSamples) {
                internal.deltas.shift();
            }

            if (internal.deltas.length > 0) {
                console.log("Tap samples:", internal.deltas);
                let average = 0.0;
                internal.deltas.forEach((delta) => { average += delta; });
                average /= internal.deltas.length;
                measured(average); // signal
            }
        }
    }

    text: qsTr("Tap Tempo")
    font.bold: false
    display: AbstractButton.TextBesideIcon
    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
}
