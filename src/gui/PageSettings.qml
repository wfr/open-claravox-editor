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

import "components"

// TODO: adapt to window size instead of fixed layout

Item {
    width: 900
    height: 700

    RowLayout {
        RoundPane {
            spacing: 16
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Material.elevation: 4
            Layout.fillHeight: true
            ColumnLayout {
                Layout.fillHeight: true
                TabButton {
                    text: "MIDI"
                    onClicked: stackLayout.currentIndex = 0
                    checked: true
                }
                TabButton {
                    text: "Global"
                    onClicked: stackLayout.currentIndex = 1
                }
                TabButton {
                    text: "App"
                    onClicked: stackLayout.currentIndex = 2
                }
                TabButton {
                    text: "App MIDI"
                    onClicked: stackLayout.currentIndex = 3
                }
                TabButton {
                    text: "Information"
                    onClicked: stackLayout.currentIndex = 4
                }
            }
        }

        RoundPane {
            Material.elevation: 4
            spacing: 16
            StackLayout {
                id: stackLayout
                PageSettingsMIDI {}
                PageSettingsGlobal {}
                PageSettingsApp {}
                PageSettingsAppMIDI {}
                PageSettingsInformation {}
            }
        }

    } // top RowLayout
}


