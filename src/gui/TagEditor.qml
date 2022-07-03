// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.15
import Qt.labs.qmlmodels 1.0

import Claravox.Backend 1.0

import "components"

// This component is ugly, both visually and in code. I don't like the imperative style.
// TODO:
// - rework in declarative style (get rid of updateButtons())
// - the TagEditor should not depend on the backend,
//   but instead offer appropriate properties, slots and signals.

Item {
    id: root
    width: 250
    height: 150

    property bool editing: false
    readonly property int maxTags: 3

    function updateButtons() {
        if (Backend.currentPreset.isFactory) {
            editing = false;
        }
        if (editing) {
            let numTags = Backend.currentPreset.tagList.rowCount();
            console.log("TagEditor: updateButtons()")
            console.log("numTags ", numTags, " of ", maxTags);
            for (let i = 0; i < repeater1.count; i++) {
                let btn = repeater1.itemAt(i);
                btn.checked = Backend.currentPreset.hasTag(btn.text);
                btn.enabled = btn.checked || (numTags < maxTags);
            }
            newTagTextField.enabled = numTags < maxTags;
        }
    }

    function onButtonToggled(tagName, checked) {
        if (Backend.presets.setTag(Backend.currentPreset.name, tagName, checked)) {
            updateButtons();
        }
    }

    onEditingChanged: {
        stackLayout.currentIndex = editing
        updateButtons();
    }

    Connections {
        target: Backend.currentPreset
        function onTagListChanged() {
            let numTags = Backend.currentPreset.tagList.rowCount();
            console.log("TagEditor: onTagListChanged(), numTags = ", numTags);
            updateButtons();
        }
    }

    Connections {
        target: Backend
        function onCurrentPresetChanged() {
            editing = false;
        }
    }

    StackLayout {
        id: stackLayout
        // ----- NOT EDITING -----
        ColumnLayout {
            spacing: 0
            Repeater {
                model: Backend.currentPreset.tagList
                Button {
                    text: model.display
                    Layout.fillWidth: true
                    enabled: false
                }
            }
        }

        // ----- EDITING -----
        Flickable {
            clip: true

            Layout.fillHeight: true
            Layout.fillWidth: true
            contentHeight: grid.height
            width: 250
            height: 150

            flickableDirection: Flickable.VerticalFlick

            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.DragOverBounds

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }

            ColumnLayout {
                TextField {
                    id: newTagTextField
                    Layout.fillWidth: true
                    placeholderText: "New Tag +"
                    validator: RegularExpressionValidator { regularExpression: /^[0-9A-Za-z +$^]{1,19}$/ }
                    onAccepted: {
                        if (Backend.presets.setTag(Backend.currentPreset.name, text, true)) {
                            enabled = false;
                            focus = false;
                            text = "";
                            root.updateButtons();
                        }
                    }
                }

                GridLayout {
                    id: grid
                    columns: 2
                    rowSpacing: 0
                    Repeater {
                        id: repeater1
                        model: Backend.tags
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
                            checked: false
                            enabled: false
                            onToggled: {
                                root.onButtonToggled(model.tag, checked);
                            }
                        }
                        onItemAdded: {
    //                        console.log("TagEditor repeater1 onItemAdded");
                        }
                        onItemRemoved: {
    //                        console.log("TagEditor repeater1 onItemRemoved");
                        }                }
                } // GridLayout (tag selection)
            } // ColumnLayout
        } // Flickable (Edit Mode)
    } // StackLayout
}
