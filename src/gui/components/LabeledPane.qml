// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

RoundPane {
    id: root
    default property alias contents: childLayout.data
    property string title: "Title"

    spacing: 16
    Material.elevation: 4
    padding: 0
    radius: 8

    ColumnLayout {
        Rectangle {
            color: "#f0f0f0"
            height: titletext.height + 8
            Layout.fillWidth: true
            radius: root.radius

            // Cover up bottom-left rounded corner
            Rectangle {
                width: parent.radius
                height: parent.radius
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                }
                color: parent.color
            }

            // Cover up bottom-right rounded corner
            Rectangle {
                width: parent.radius
                height: parent.radius
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                }
                color: parent.color
            }

            Text {
                id: titletext
                text: root.title
                font.pixelSize: 18
                anchors.verticalCenter: parent.verticalCenter
                padding: 8
                leftPadding: 12
                rightPadding: 12
            }
        }

        Pane {
            id: pane
            background: Rectangle {
                radius: root.radius
            }

            ColumnLayout {
                id: childLayout
            }
        }
    }
}
