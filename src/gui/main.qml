// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.12
import QtQml.Models 2.12

import "components"
import Claravox.Backend 1.0

ApplicationWindow {
    id: window
    //    visibility: Window.Maximized
    y: 0
    width: 1200
    height: 900
    visible: true
    title: qsTr("Open Claravox Editor")

    color: "#f0f0f0"
    Material.accent: Backend.uiAccentColor

    Dialog {
        id: unsavedWarningDialog
        modal: true
        title: "WARNING"
        standardButtons: Dialog.Ok | Dialog.Cancel
        Component.onCompleted: visible = false
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        property var discard_func
        Text {
            text: "Changing presets will cause any unsaved changes to be lost. This cannot be undone."
        }
        onRejected: {
            close()
        }
        onAccepted: {
            close()
            discard_func()
        }
    }

    // https://doc.qt.io/qt-5/qml-qtquick-controls2-toolbar.html
    header: ToolBar {
        Material.primary: Material.background
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("CLONE")
                onClicked: {
                    Backend.cloneCurrentPreset()
                }
            }
            ToolButton {
                text: qsTr("SAVE")
                onClicked: {
                    Backend.saveCurrentPreset()
                }
            }

            ComboBox {
                id: comboBoxPreset
                Layout.fillWidth: true
                model: Backend.sortedPresets
                currentIndex: Backend.currentSortedPresetIndex
                textRole: "name"
                displayText: currentText + (Backend.currentPresetModified ? " *" : "")
//                font.italic: Backend.currentPresetModified
                font.pointSize: 12
                onCurrentIndexChanged: {
                    console.log("Selected (sorted) preset index", currentIndex);
                    Backend.currentSortedPresetIndex = currentIndex;
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
                id: buttonPrev
                text: "<"
                font.pixelSize: 18
                onClicked: {
                    if (Backend.currentPresetModified) {
                        unsavedWarningDialog.discard_func = () => {
                            comboBoxPreset.decrement();
                        }
                        unsavedWarningDialog.open();
                    } else {
                        comboBoxPreset.decrement();
                    }
                }
            }
            Button {
                id: buttonNext
                text: ">"
                font.pixelSize: 18
                onClicked: {
                    if (Backend.currentPresetModified) {
                        unsavedWarningDialog.discard_func = () => {
                            comboBoxPreset.increment();
                        }
                        unsavedWarningDialog.open();
                    } else {
                        comboBoxPreset.increment();
                    }
                }
            }

            TabButton {
                id: tabButton0
                text: qsTr("Editor")
                onClicked: stackLayout.currentIndex = 0
                checked: true
                ToolTip.text: "Alt+1"
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }
            TabButton {
                id: tabButton1
                text: qsTr("Library")
                onClicked: stackLayout.currentIndex = 1
                ToolTip.text: "Alt+2"
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }
            TabButton {
                id: tabButton2
                text: qsTr("Settings")
                onClicked: stackLayout.currentIndex = 2
                ToolTip.text: "Alt+3"
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }

//            Label {
//                text: Backend.connected ? "Connected" : "Disconnected"
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

    Shortcut {
        sequence: "Left"
        onActivated: {
            buttonPrev.clicked()
        }
    }
    Shortcut {
        sequence: "Right"
        onActivated: {
            buttonNext.clicked()
        }
    }
    Shortcut {
        sequence: "Up"
        onActivated: {
            buttonPrev.clicked()
        }
    }
    Shortcut {
        sequence: "Down"
        onActivated: {
            buttonNext.clicked()
        }
    }
    Shortcut {
        sequence: "Alt+1"
        onActivated: {
            tabButton0.checked = true
            tabButton0.clicked()
        }
    }
    Shortcut {
        sequence: "Alt+2"
        onActivated: {
            tabButton1.checked = true
            tabButton1.clicked()
        }
    }
    Shortcut {
        sequence: "Alt+3"
        onActivated: {
            tabButton2.checked = true
            tabButton2.clicked()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
