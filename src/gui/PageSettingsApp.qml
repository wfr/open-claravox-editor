// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import QtQuick.Controls.Universal 2.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Dialogs 1.3

import "components"
import Claravox.BackEnd 1.0

Item {
    width: 800

    ColorDialog {
        id: colorDialog
        title: "Please choose an accent color"

        onCurrentColorChanged: {
            window.Material.accent = colorDialog.currentColor;
        }
        onAccepted: {
            window.Material.accent = colorDialog.color;
            BackEnd.uiAccentColor = colorDialog.color;
        }
        onRejected: {
            window.Material.accent = colorDialog.color
        }
        Component.onCompleted: {
            colorDialog.visible = false
        }
    }

    ColumnLayout {
        width: 800
        SectionLabel {
            text: "Appearance"
        }
        RowLayout {
            spacing: 8
            ColumnLayout {
                spacing: 0
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                Label {
                    text: "UI Color Mode"
                }
                RowLayout {
                    spacing: 0
                    Button {
                        text: "Dark"
                        enabled: false // TODO: implement
                        Layout.fillWidth: true
                        autoExclusive: true
                        checkable: true
                    }
                    Button {
                        text: "Light"
                        enabled: false // TODO: implement
                        Layout.fillWidth: true
                        autoExclusive: true
                        checkable: true
                        checked: true
                    }
                }
                Button {
                    text: "Accent Color"
                    Layout.fillWidth: true
                    onClicked: {
                        colorDialog.color = Material.accent
                        colorDialog.open();
                    }
                }
            }
            ColumnLayout {
                spacing: 0
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                Label {
                    text: "Show Number Values"
                }
                RowLayout {
                    spacing: 0
                    Button {
                        text: "Show"
                        enabled: false // TODO: implement
                        Layout.fillWidth: true
                        autoExclusive: true
                        checkable: true
                        checked: true
                    }
                    Button {
                        text: "Hide"
                        enabled: false // TODO: implement
                        Layout.fillWidth: true
                        autoExclusive: true
                        checkable: true
                    }
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }
        Item {
            height: 16
        }

        SectionLabel {
            text: "Factory Settings"
        }
        RowLayout {
            spacing: 8
            Button {
                text: "Diagnostic Mode"
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                enabled: false // TODO: implement
            }
            Button {
                text: "Restore Factory Presets"
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                enabled: false // TODO: implement
            }
        }

        Item {
            height: 16
        }

        GridLayout {
            columns: 2
            columnSpacing: 8
            SectionLabel {
                text: "Tag Management"
            }
            SectionLabel {
                text: "Preset Management"
            }
            Button {
                text: "Delete Tags"
                Layout.fillWidth: true
                Layout.preferredWidth: 400
                enabled: false // TODO: implement
            }
            ColumnLayout {
                spacing: 0
                Label {
                    text: "Unsaved Changes"
                }
                RowLayout {
                    spacing: 0
                    Button {
                        text: "Warn"
                        Layout.preferredWidth: 200
                        checkable: true
                        checked: true
                        enabled: false // TODO: implement
                    }
                    Button {
                        text: "Ignore"
                        Layout.preferredWidth: 200
                        Layout.fillWidth: true
                        checkable: true
                        enabled: false // TODO: implement
                    }
                }
            }
        }
    }
}
