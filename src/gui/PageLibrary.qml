// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import Qt.labs.qmlmodels 1.0

import Claravox.BackEnd 1.0

import "components"

// TODO: adapt to window size instead of fixed layout

Item {
    width: 900
    height: 600

    visible: true

    GridLayout {
        columns: 2
        rows: 2

        // -------------- Top left --------------
        RoundPane {
            id: roundPaneTopLeft
            contentWidth: 858
            contentHeight: 550
            radius: 8
            Material.elevation: 4
            Layout.alignment: Qt.AlignTop

            QtObject {
                id: internal
                readonly property int listWidth: 832
                readonly property int lineHeight: 48
            }

            ColumnLayout {
                id: listColumnLayout
                spacing: 0

                RowLayout {
                    Button {
                        id: buttonNewPreset
                        text: qsTr("New Preset +")
                    }
                    Button {
                        id: buttonReceiveTimbre
                        text: qsTr("Receive Timbre 1")
                    }
                }

                // Leave the header outside of the ListView to avoid overlapping with list items.
                Rectangle {
                    id: listHeader
                    width: internal.listWidth
                    height: 24
                    z: 2
                    color: Material.backgroundColor
                    Layout.topMargin: 8

                    component HeaderText : Text {
                        height: 20
                        color: Material.secondaryTextColor
                        font.pointSize: 10
                        verticalAlignment: Text.AlignVCenter
                    }

                    Row {
                        Item {
                            width: 10
                            height: parent.height
                        }

                        HeaderText {
                            width: 290
                            text: "PRESET NAME"
                        }
                        HeaderText {
                            width: 300
                            text: "TAGS"
                        }
                        HeaderText {
                            width: 200
                            text: "GROUP"
                        }
                    }
                }

                Rectangle {
                    width: listHeader.width
                    height: 1
                    color: Material.secondaryTextColor
                }

                ListView {
                    id: presetListView
                    model: BackEnd.presets
                    // model: BackEnd.filteredPresets
                    currentIndex: BackEnd.currentPresetIndex

                    topMargin: 0
                    // Ugly layout
                    Layout.preferredWidth: roundPaneTopLeft.contentWidth
                    Layout.preferredHeight: roundPaneTopLeft.contentHeight - buttonNewPreset.height - listHeader.height - 16
                    clip: true

                    delegate: Item {
                        width: internal.listWidth
                        height: internal.lineHeight

                        component StyledText : Text {
                            height: internal.lineHeight
                            color: Material.foreground
                            font.pointSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }


                        Row {
                            Item {
                                width: 10
                                height: parent.height
                            }

                            StyledText {
                                width: 290
                                text: model.name
                            }
                            StyledText {
                                width: 300
                                text: model.tagstring
                            }
                            StyledText {
                                width: 200
                                text: model.group
                            }
                            StyledText {
                                width: 32
                                text: model.favorite ? "★" : ""
                                font.pointSize: 20
//                                color: "#FFC000" // gold
                                color: Material.accent
                            }
                        }

                        Rectangle {
                            height: 1
                            width: parent.width
                            color: Material.dividerColor
                            x: 0
                            y: internal.lineHeight
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                let preset = BackEnd.presets.byIndex(index);
                                console.log("clicked preset", index, preset.name);
                                BackEnd.selectPreset(preset.name);
                            }
                        }
                    }

                    highlight: RowLayout {
                        Rectangle {
                            color: Material.accentColor
                            width: 4
                            height: internal.lineHeight
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }

                    highlightMoveDuration: 250

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AlwaysOn
                    }

                }

                Rectangle {
                    width: listHeader.width
                    height: 1
                    color: Material.secondaryTextColor
                }
            }
        }

        // -------------- Top right --------------
        RoundPane {
            id: topRightPane
            radius: 8
            Material.elevation: 4
            Layout.preferredHeight: roundPaneTopLeft.height
            Layout.alignment: Qt.AlignTop
            width: 280
            contentWidth: 280

            Flickable {
                clip: true
                Layout.preferredWidth: 280
                width: 280
                contentWidth: 280
                height: parent.height
                contentHeight: columnLayoutTR.childrenRect.height

                maximumFlickVelocity: 800

//                boundsMovement: Flickable.StopAtBounds
//                boundsBehavior: Flickable.DragOverBounds

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    policy: ScrollBar.AlwaysOn
                }
                Keys.onUpPressed: scrollBar.decrease()
                Keys.onDownPressed: scrollBar.increase()

                ColumnLayout {
                    id: columnLayoutTR
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: 248
                    width: 248

                    Button {
                        text: "Favorite"
                        checkable: true
                        Layout.fillWidth: true
                    }

                    SectionLabel {
                        text: "Groups"
                    }

                    ColumnLayout {
                        Layout.minimumWidth: 250
                        spacing: 0
                        Repeater {
                            model: BackEnd.groups
                            Button {
                                text: model.group
                                Layout.fillWidth: true
                                checkable: true
                                spacing: 0
                            }
                        }
                    }

                    SectionLabel {
                        text: "Tags"
                    }

                    GridLayout {
                        Layout.minimumWidth: 250
                        columns: 2
                        rowSpacing: 0
                        Repeater {
                            model: BackEnd.tags
                            Button {
                                text: model.tag
                                Layout.fillWidth: true
                                checkable: true
                                padding:  {
                                    top: 0
                                    right: 0
                                    bottom: 0
                                    left: 0
                                }
                            }
                        }
                    }
                }
            } // Flickable
        } // RoundPane

        // -------------- Bottom --------------
        RoundPane {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            radius: 8
            Material.elevation: 4

            RowLayout {
                anchors.fill: parent

                // Left section
                GridLayout {
                    columns: 2
                    Layout.fillWidth: true
                    ColumnLayout {
                        Layout.columnSpan: 2
                        SectionLabel {
                            text: "Loaded Preset Name"
                        }
                        RowLayout {
                            Layout.columnSpan: 2
                            TextInput {
                                text: BackEnd.currentPreset.name
                                font.pointSize: 16
                                color: Material.primaryTextColor
                                onAccepted: {
                                    BackEnd.renameCurrentPreset(text);
                                }
                            }
                            Button {
                                text: "Edit"
            //                    visible: !BackEnd.currentPreset.isFactory
                            }
                        }
                    }

                    Column {
                        SectionLabel {
                            text: "Save Date"
                        }
                        Text {
                            text: BackEnd.currentPreset.lastModified
                            color: Material.primaryTextColor
                        }
                    }
                    Column {
                        SectionLabel {
                            text: "Group"
                        }
                        Text {
                            text: BackEnd.currentPreset.group
                            color: Material.primaryTextColor
                        }
                    }
                    RowLayout {
                        Button {
                            text: "Favorite"
                            onPressed: {
                                BackEnd.presets.toggleFavorite(BackEnd.currentPreset.name);
                            }
                        }
                        Button {
                            text: "Open folder"
                            onClicked: {
                                Qt.openUrlExternally(BackEnd.presetsFolderURL());
                            }
                        }
                        Button {
                            text: "Delete"
                        }
                    }
                }

                // Right section (tags)
                ColumnLayout {
                    Layout.preferredWidth: 250
                    spacing: 0
                    SectionLabel {
                        text: "Tags"
                    }
                    Repeater {
                        model: BackEnd.currentPreset.tagList
                        Button {
                            text: model.display
                            Layout.fillWidth: true
                            checkable: true
                        }
                    }
                }
            }
        }
    }
}


