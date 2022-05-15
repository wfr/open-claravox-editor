// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
//import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1

import "components"
import Claravox.BackEnd 1.0

ApplicationWindow {
    id: window
    //    visibility: Window.Maximized
//    x: 0
    y: 0
    width: 1200
    height: 900
    visible: true
    title: qsTr("Open Claravox Editor")
    //    color: "#1f2120"

    color: "#f0f0f0"
//    Material.accent: "#0f0" // TODO: make configurable
    Material.accent: BackEnd.uiAccentColor

    MessageDialog {
        id: messageDialogWarningUnsaved
        title: "WARNING"
        text: "Changing presets will cause any unsaved changes to be lost. This cannot be undone."
//        icon: StandardIcon.Warning
        buttons: StandardButton.Discard | StandardButton.Cancel
        property var discard_func
        onRejected: {
            close()
        }
        onDiscardClicked: {
            close()
            discard_func()
        }
        Component.onCompleted: visible = false
    }

    // https://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
    header: ToolBar {
        Material.primary: Material.background
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("CLONE")
            }
            ToolButton {
                text: qsTr("SAVE")
                onClicked: {
                    BackEnd.saveCurrentPreset()
                }
            }

            ComboBox {
                id: comboBoxPreset
                Layout.fillWidth: true
                model: BackEnd.presets
                currentIndex: BackEnd.currentPresetIndex
                textRole: "name"
                displayText: currentText + (BackEnd.currentPresetModified ? " *" : "")
//                font.italic: BackEnd.currentPresetModified
                font.pointSize: 12
                onCurrentIndexChanged: {
                    console.log("Selected preset index", currentIndex);
                    let name = BackEnd.presets.byIndex(currentIndex).name;
                    if (name.length > 0) {
                        BackEnd.selectPreset(name);
                    }
                }

                function decrement() {
                    if (currentIndex > 0) {
                        decrementCurrentIndex();
                    } else {
                        currentIndex = count - 1;
                    }
                }

                function increment() {
                    if (currentIndex < count - 1) {
                        incrementCurrentIndex();
                    } else {
                        currentIndex = 0;
                    }
                }
            }

            Button {
                text: "<"
                font.pixelSize: 18
                onPressed: {
                    if (BackEnd.currentPresetModified) {
                        messageDialogWarningUnsaved.discard_func = () => {
                            comboBoxPreset.decrement();
                        }
                        messageDialogWarningUnsaved.open();
                    } else {
                        comboBoxPreset.decrement();
                    }
                }
            }
            Button {
                text: ">"
                font.pixelSize: 18
                onPressed: {
                    if (BackEnd.currentPresetModified) {
                        messageDialogWarningUnsaved.discard_func = () => {
                            comboBoxPreset.increment();
                        }
                        messageDialogWarningUnsaved.open();
                    } else {
                        comboBoxPreset.increment();
                    }
                }
            }

            TabButton {
                text: qsTr("Editor")
                onClicked: stackLayout.currentIndex = 0
                checked: true
            }
            TabButton {
                text: qsTr("Library")
                onClicked: stackLayout.currentIndex = 1
            }
            TabButton {
                text: qsTr("Settings")
                onClicked: stackLayout.currentIndex = 2
            }

//            Label {
//                text: BackEnd.connected ? "Connected" : "Disconnected"
//            }
        }
    }

    StackLayout {
        id: stackLayout
        currentIndex: 0
        anchors.fill: parent

        PageEditor {
        }

        PageLibrary {
        }

        PageSettings {
        }
    }

//    PageIndicator {
//        id: indicator

//        count: stackLayout.count
//        currentIndex: stackLayout.currentIndex

//            anchors.bottom: parent.bottom
//            anchors.horizontalCenter: parent.horizontalCenter
//    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
