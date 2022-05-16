// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.15

import "components"

import Claravox.Backend 1.0

ColumnLayout {
    width: 1400
    height: 900

    Flickable {
        clip: true

//        height: parent.height
//        height: parent.height - modAssignments.height
        Layout.fillHeight: true
        Layout.fillWidth: true

//        contentHeight: flow1.childrenRect.height
        contentHeight: editorContents.childrenRect.height
        bottomMargin: 16

        // Fixed-width layout (like Moog)
        // ... this could be configurable in the future.
//        width: 1050
        width: parent.width
//        anchors.centerIn: parent
//        contentWidth: 1050

        // Alternative flowing layout
        // ... allows the editor to fit the screen without scrolling
//        width: parent.width
//        contentWidth: parent.width

        // disable annoying bounce
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.DragOverBounds

        // add ScrollBar to Flickable
        Keys.onUpPressed: scrollBar.decrease()
        Keys.onDownPressed: scrollBar.increase()
        ScrollBar.vertical: ScrollBar {
            id: scrollBar
            policy: ScrollBar.AlwaysOn
        }

//        maximumFlickVelocity: 1500 // workaround for QTBUG-56075
//        flickDeceleration: 5000

        Item {
            id: wrapper
            width: 1050
            anchors.horizontalCenter: parent.horizontalCenter

            PageEditorContents {
                id: editorContents
            }
        } // wrapper
    } // Flickable

    Item {
        id: modWindow

        function toggle() {
            modAssignments.visible = !modAssignments.visible
        }

        Button {
            id: buttonToggleMods
            text: "CV MOD"
            x: parent.width - width - scrollBar.width - 4 // arbitrary
            y: -height
            onClicked: {
                modWindow.toggle()
            }
        }
        Layout.fillWidth: true
        Layout.preferredHeight: modAssignments.visible ? modAssignments.height : 0

        Item {
            id: modAssignments
            height: 200
            Rectangle {
                width: parent.width
                color: "white"
                height: 200
            }
            ColumnLayout {
                SectionLabel {
                    text: "Volume Response Sources"
                }
                RowLayout {
                    ColumnLayout {
                        Button {
                            checkable: true
                            text: "Pitch CV"
                        }
                        LabeledSlider {
                            description: "Depth"
                            unit: "%"
                            from: -100
                            to: 100
                            value: 0
                        }
                    }
                    ColumnLayout {
                        Button {
                            checkable: true
                            text: "Volume CV"
                        }
                        LabeledSlider {
                            description: "Depth"
                            unit: "%"
                            from: -100
                            to: 100
                            value: 0
                        }
                    }
                    ColumnLayout {
                        Button {
                            checkable: true
                            text: "External CV"
                        }
                        LabeledSlider {
                            description: "Depth"
                            unit: "%"
                            from: -100
                            to: 100
                            value: 0
                        }
                    }
                }
            }
        }
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
