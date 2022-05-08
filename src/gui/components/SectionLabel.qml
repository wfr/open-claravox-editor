// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Column {
    property alias text: caption.text
    Layout.fillWidth: true
    Text {
        id: caption
        color: Material.secondaryTextColor
        font.pointSize: 10
        font.capitalization: Font.AllUppercase
    }
    Rectangle {
        width: parent.width
        height: 1
        color: Material.secondaryTextColor
    }
}
