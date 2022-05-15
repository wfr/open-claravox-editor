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
NOTE:
The scales are displayed in alphabetical order,
but stored in a different order internally!
*/

Item {
    id: scaleSelector

    property real scaleVal: 0 // normalized [0..1]
    signal scaleSelected(double val)

    // Scales in the Claravox' internal order.
    //
    // We could read this list at runtime from the Backend instead,
    // but then we would lose the ability to test this UI component in isolation.
    // The firmware has this list hardcoded anyway, and it's very unlikely to change.
    readonly property var names: [
        "Chromatic",
        "Major",     // Ionian
        "Minpenta",
        "Fifth",
        "Dorian",
        "Phrygian",
        "Lydian",
        "Mixolydian",
        "Minor",     // Aeolian
        "Locrian",
        "Majblues",
        "Minblues",
        "Diminish",
        "Majpenta",
        "Spanish",
        "Romani",
        "Arabian",
        "Egyptian",
        "Ryukyu",
        "Wholetone",
        "Maj3rd",
        "Min3rd"
    ];

    readonly property var names_sorted: names.slice().sort();

    QtObject {
        id: internal
        property int scaleIndex: 0
        // map name to scaleVal [0..1]
        property var name_to_scaleindex: ({});
        // map name to wordlist index
        property var name_to_wordindex: ({});
    }

    Component.onCompleted: {
//        console.log("ScaleSelector.onCompleted");
    }

    function currentName() {
        return names[internal.scaleIndex];
    }

    function nameToScaleIndex(name) {
        // init cache
        if (Object.keys(internal.name_to_scaleindex).length == 0) {
            for (let i = 0; i < names.length; i++) {
                internal.name_to_scaleindex[names[i]] = i
            }
        }
        return internal.name_to_scaleindex[name];
    }

    function nameToWordIndex(name) {
        // init cache
        if (Object.keys(internal.name_to_wordindex).length == 0) {
            for (let i = 0; i < names_sorted.length; i++) {
                internal.name_to_wordindex[names_sorted[i]] = i;
            }
        }
        return internal.name_to_wordindex[name];
    }

    function currentWordIndex() {
        return nameToWordIndex(currentName());
    }

    Connections {
        target: scaleSelector
        function onScaleValChanged() {
            internal.scaleIndex = Math.round(scaleVal * names.length);
            if (internal.scaleIndex < 0 || internal.scaleIndex > names.length) {
                console.log("ERROR internal.scaleIndex", internal.scaleIndex);
            }
        }
    }

    Connections {
        target: internal
        function onScaleIndexChanged() {
            wordlist.selectedIndex = currentWordIndex();
            scaleSelected(internal.scaleIndex / names.length);
        }
    }

    TabWordList {
        id: wordlist
        selectedIndex: parent.wordlistIndex();
        words: names_sorted
        onWordSelected: (i) => {
                            console.log("TabWordList onWordSelected:", selectedWord());
                            internal.scaleIndex = nameToScaleIndex(selectedWord());
                        }
    }

    width: childrenRect.width
    height: childrenRect.height
}




