// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15

/*
  The buttons in this component are not generated dynamically on purpose,
  because that way the component renders nicely in the UI designer.
  Also the number of MIDI channels is constant anyway.
*/

Item { 
    property int channel: 1
//    property alias omni: button0.visible
    property bool omni: true

    // not sure why this breaks the parent layout
//    width: childrenRect.width
//    height: childrenRect.height
    width: 256
//    height: 190 + 50 * omni
    height: 240

    onChannelChanged: {
        console.log("MidiChannel", channel, "selected");
        buttonGrid.children[channel].checked = true;
    }

    onOmniChanged: {
        if (omni) {
            button0.opacity = 1.0
            button0.enabled = true
        } else {
            button0.opacity = 0.0
            button0.enabled = false
        }
    }

    ButtonGroup {
        buttons: buttonGrid.children
    }

    GridLayout {
        id: buttonGrid
        rowSpacing: 0
        columnSpacing: 0
        rows: 5
        columns: 4
        width: 256

        Button {
            id: button0
            text: qsTr("Omni")
            Layout.fillWidth: true
            Layout.columnSpan: 4
            checkable: true
            checked: false
            onClicked: channel = 0
        }

        Button {
            id: button1
            text: qsTr("1")
            checkable: true
            checked: true
            onClicked: channel = 1;
        }

        Button {
            id: button2
            text: qsTr("2")
            checkable: true
            onClicked: channel = 2;
        }

        Button {
            id: button3
            text: qsTr("3")
            checkable: true
            onClicked: channel = 3;
        }

        Button {
            id: button4
            text: qsTr("4")
            checkable: true
            onClicked: channel = 4;
        }

        Button {
            id: button5
            text: qsTr("5")
            checkable: true
            onClicked: channel = 5;
        }

        Button {
            id: button6
            text: qsTr("6")
            checkable: true
            onClicked: channel = 6;
        }

        Button {
            id: button7
            text: qsTr("7")
            checkable: true
            onClicked: channel = 7;
        }

        Button {
            id: button8
            text: qsTr("8")
            checkable: true
            onClicked: channel = 8;
        }

        Button {
            id: button9
            text: qsTr("9")
            checkable: true
            onClicked: channel = 9;
        }

        Button {
            id: button10
            text: qsTr("10")
            checkable: true
            onClicked: channel = 10;
        }

        Button {
            id: button11
            text: qsTr("11")
            checkable: true
            onClicked: channel = 11;
        }

        Button {
            id: button12
            text: qsTr("12")
            checkable: true
            onClicked: channel = 12;
        }

        Button {
            id: button13
            text: qsTr("13")
            checkable: true
            onClicked: channel = 13;
        }

        Button {
            id: button14
            text: qsTr("14")
            checkable: true
            onClicked: channel = 14;
        }

        Button {
            id: button15
            text: qsTr("15")
            checkable: true
            onClicked: channel = 15;
        }

        Button {
            id: button16
            text: qsTr("16")
            checkable: true
            onClicked: channel = 16;
        }
    }
}


