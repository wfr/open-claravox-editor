// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.0

/*
A tabbed word cloud, mostly hardcoded for a single use.

In the unlikely case this ever gets used somewhere else,
feel free to generalize it properly.
*/

Item {
    id: root

    property var words: []
    property int selectedIndex: 0
    signal wordSelected(int i)

    function selectedWord() {
        return words[selectedIndex];
    }

    width: 482 // TODO: parametrize globally (issue #8)
    height: 179

    // DEBUG
//    Rectangle {
//        width: parent.width
//        height: parent.height
//        border.color: "#f00"
//        color: "transparent"
//    }

    QtObject {
        id: internal
        /* Words will be divided into the following ranges.
           This is hardcoded for now, but could be generated dynamically. */
        readonly property var ranges: [
            'A-L',
            'M-O',
            'P-Z'
        ]

        // List of wordlists, one per range
        property var buckets: []
    }

    function fillBuckets() {
        internal.buckets = []
        internal.ranges.forEach((range) => {
            let matches = [];
            words.forEach((word) => {
                let c = word.toUpperCase()[0];
                if (c >= range[0] && c <= range[2]) {
                    matches.push(word);
                }
            });
            internal.buckets.push(matches);
        });
//        console.log("TabWordList buckets:", internal.buckets);
    }

    function wordIndex(page, localindex) {
        let id = 0;
        for (let p = 0; p < page; p++) {
            id += internal.buckets[p].length;
        }
        id += localindex;
        return id;
    }

    onWordsChanged: {
        fillBuckets();
        selectedIndex = 0;
    }

    onSelectedIndexChanged: {
        let pageIndex;
        let localIndex;

        let from, to = 0;
        for (let p = 0; p < internal.buckets.length; p++) {
            from = to;
            to = from + internal.buckets[p].length;
            if (selectedIndex >= from && selectedIndex <= to) {
                pageIndex = p;
                localIndex = selectedIndex - from;
            }
        }
//        console.log("TabWordList selectedIndex:", selectedIndex, "pageIndex:", pageIndex, "localIndex:", localIndex);

        // update UI
        textSelectedWord.text = words[selectedIndex];
        tabBar.currentIndex = pageIndex;
        stackLayout.currentIndex = pageIndex;
        pageRepeater.itemAt(pageIndex).children[localIndex].checked = true;

        // signal
        root.wordSelected(selectedIndex)
    }

    ButtonGroup {
        id: wordButtonGroup
    }

    ColumnLayout {
        height: 210

        RowLayout {
            width: root.width
            Layout.preferredWidth: root.width

            Text {
                id: text1
                color: Material.foreground
                text: qsTr("Scale")
                font.pixelSize: 16
                textFormat: Text.PlainText
            }

            Text {
                id: textSelectedWord
                text: qsTr("Chromatic")
                font.pixelSize: 20
                textFormat: Text.PlainText
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                color: Material.foreground
            }
        }

        TabBar {
            id: tabBar
            Layout.preferredWidth: root.width

            Repeater {
                id: tabButtonRepeater
                model: internal.ranges
                delegate: TabButton {
                    text: modelData
                    font.pixelSize: 16
                    onClicked: {
                        stackLayout.currentIndex = index;
                    }
                }
            }

//            contentHeight: 32
        }

        StackLayout {
            id: stackLayout
            width: root.width
            Layout.preferredWidth: root.width
            Layout.preferredHeight: 100
            currentIndex: tabBar.currentIndex

            Repeater {
                id: pageRepeater
                model: internal.ranges
                delegate: GridLayout {
                    property int pageIndex: index
                    property int numWords: internal.buckets[index].length

                    height: 85
                    Layout.preferredWidth: root.width
                    Layout.fillWidth: true
                    rows: 2
                    columns: numWords > 8 ? 5 : 4 // ugly, I know

                    Repeater {
                        id: wordRepeater
                        model: internal.buckets[pageIndex]
                        delegate: Button {
                            text: modelData
                            onClicked: {
                                selectedIndex = wordIndex(pageIndex, index)
//                                console.log("TabWordList delegate.onClicked", selectedIndex, modelData);
                            }
                            checkable: true
                            checked: false
                            ButtonGroup.group: wordButtonGroup
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            font.bold: false
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
    }
}
