// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.15

import "components"

import Claravox.BackEnd 1.0

Item {
    width: 1400
    height: 900

    Flickable {
        clip: true

        height: parent.height
        contentHeight: flow1.childrenRect.height
        bottomMargin: 16

        // Fixed-width layout (like Moog)
        // ... this could be configurable in the future.
//        width: 1120
//        anchors.centerIn: parent
//        contentWidth: 1100

        // Alternative flowing layout
        // ... allows the editor to fit the screen without scrolling
        width: parent.width
        contentWidth: parent.width

        // disable annoying bounce
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.DragOverBounds

        // add ScrollBar to Flickable
        Keys.onUpPressed: scrollBar.decrease()
        Keys.onDownPressed: scrollBar.increase()
        ScrollBar.vertical: ScrollBar { id: scrollBar }

        Flow {
            id: flow1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            rightPadding: 5
            leftPadding: 5
            bottomPadding: 5
            topPadding: 5
            spacing: 10
            layoutDirection: Qt.LeftToRight
            flow: Flow.LeftToRight
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0

            LabeledPane {
                title: qsTr("Antennas")
                id: paneAntennas
                ColumnLayout {
                    LabeledSlider {
                        id: sliderVolumeResponse
                        description: "Volume Response"
                        unit: ""
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                        value: BackEnd.params.volCurveVal
                        onValueChanged: BackEnd.params.volCurveVal = value
                    }

                    LabeledSlider {
                        id: sliderPitchResponse
                        description: "Pitch Response"
                        unit: ""
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                        value: BackEnd.params.pitchCurveVal
                        onValueChanged: BackEnd.params.pitchCurveVal = value
                    }
                }
            }

            LabeledPane {
                id: paneQuantization
                title: qsTr("Quantize")

                ColumnLayout {
                    x: 0
                    y: 0

                    LabeledSlider {
                        id: sliderQuantizeAmount
                        unit: ""
                        description: "Amount"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                        value: BackEnd.params.quantizeAmount
                        onValueChanged: BackEnd.params.quantizeAmount = value
                    }

                    LabeledSlider {
                        id: sliderRootNote
                        unit: ""
                        from: 0
                        to: 11
                        stepSize: 1
                        description: "Root"
                        value: denormalize(BackEnd.params.rootVal)
                        onValueChanged: BackEnd.params.rootVal = normalize(value)

                        function renderValue() {
                            let note = Math.round(this.value);
                            switch(note) {
                            case 0:
                                return "C";
                            case 1:
                                return "C#";
                            case 2:
                                return "D";
                            case 3:
                                return "D#";
                            case 4:
                                return "E";
                            case 5:
                                return "F";
                            case 6:
                                return "F#";
                            case 7:
                                return "G";
                            case 8:
                                return "G#";
                            case 9:
                                return "A";
                            case 10:
                                return "A#";
                            case 11:
                                return "B";
                            default:
                                console.log("ERROR: sliderRootNote invalid", note);
                            }
                        }
                    }
                }
            }

            LabeledPane {
                id: paneQuantizeScale
                title: qsTr("Quantize Scale")

                ColumnLayout {
                    x: 0
                    y: 0
                    ScaleSelector {
                        id: scaleSelector
                        scaleVal: BackEnd.params.scaleVal
                        onScaleSelected: (v) => {
                             BackEnd.params.scaleVal = v;
                         }
                    }
                }
            }

            LabeledPane {
                id: labeledPaneTimbre
                title: qsTr("Timbre")

                GridLayout {
                    columnSpacing: 34 // TODO: parametrize (issue #8)
                    rows: 2
                    columns: 2

                    Register {
                        id: registerSelector
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        value: BackEnd.params.octaveState
                        // custom signal to avoid breaking bindings:
                        onValueSelected: (v) => { BackEnd.params.octaveState = v };
                    }

                    LabeledSlider {
                        id: sliderFilter
                        description: "Filter\nCutoff"
                        unit: ""
                        function renderValue() {
                            let trigger_binding = value; // dummy
                            let hz = BackEnd.params.getFilterValHz();
                            if (hz < 999.5) {
                                return Number(hz).toFixed(0);
                            } else {
                                return Number(hz / 1000.0).toFixed(2);
                            }
                        }
                        function renderUnit() {
                            let trigger_binding = value; // dummy
                            let hz = BackEnd.params.getFilterValHz();
                            if (hz < 999.5) {
                                return "Hz";
                            } else {
                                return "kHz";
                            }
                        }
                        value: BackEnd.params.filterVal
                        onValueChanged: BackEnd.params.filterVal = value
                    }

                    LabeledSlider {
                        id: sliderBrightness
                        unit: ""
                        description: "Brightness"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                        value: BackEnd.params.brightVal
                        onValueChanged: BackEnd.params.brightVal = value
                    }

                    LabeledSlider {
                        id: sliderWaveAdjust
                        Layout.columnSpan: 1
                        unit: ""
                        description: "Wave"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                        value: BackEnd.params.waveVal
                        onValueChanged: BackEnd.params.waveVal = value
                    }
                }

            }

            LabeledPane {
                id: labeledPaneDelay
                title: qsTr("Delay")

                property alias delayTime: sliderDelayTime.value
                property alias mix: sliderMix.value
                property alias feedback: sliderFeedback.value

                GridLayout {
                    columnSpacing: 34 // TODO: parametrize (issue #8)
                    rows: 2
                    columns: 2

                    LabeledSlider {
                        id: sliderDelayTime
                        description: "Delay Time"
                        unit: "ms"
                        function renderValue() {
                            let ms = _interpolate(value, from, to, 70, 700);
                            return Number(ms).toFixed(0);
                        }
                        value: BackEnd.params.timeVal
                        onValueChanged: BackEnd.params.timeVal = value
                    }

                    LabeledSlider {
                        id: sliderMix
                        description: "Mix"
                        unit: "%"
                        function renderValue() {
                            let pct = _interpolate(value, from, to, 0, 100);
                            return Number(pct).toFixed(0);
                        }
                        value: BackEnd.params.delayAmount
                        onValueChanged: BackEnd.params.delayAmount = value
                    }

                    TapTimeButton {
                        onMeasured: (delta) => {
                            let time = Math.max(70, Math.min(700, Math.round(delta)));
                            BackEnd.params.timeVal = _interpolate(time, 70, 700, 0.0, 1.0);
                        }
                    }

                    LabeledSlider {
                        id: sliderFeedback
                        description: "Feedback"
                        unit: ""
                        function renderValue() {
                            let pct = _interpolate(value, from, to, 0, 100);
                            return Number(pct).toFixed(0);
                        }
                        value: BackEnd.params.feedbackVal
                        onValueChanged: BackEnd.params.feedbackVal = value
                    }
                }
            }

            LabeledPane {
                id: labeledPanePitchAntennaMod
                title: qsTr("Pitch Antenna Mod")
                GridLayout {
                    columnSpacing: 34
                    rows: 2
                    columns: 1
                    LabeledSlider {
                        id: sliderPitchModWsFreq
                        description: "Wavetable Scan Freq"
                        from: 0
                        to: 1.0
                        unit: "%"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                        }
                        value: BackEnd.params.pitchModWsFreq
                        onValueChanged: BackEnd.params.pitchModWsFreq = value
                    }
                    LabeledSlider {
                        id: sliderPitchModWsAmount
                        description: "Wavetable Scan Pos"
                        unit: "%"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                        }
                        value: BackEnd.params.pitchModWsAmount
                        onValueChanged: BackEnd.params.pitchModWsAmount = value
                    }
                }
            }


            LabeledPane {
                id: labeledPaneOsc1
                title: qsTr("Oscillator 1")
                Oscillator1 {
                    id: osc1
                    level: BackEnd.params.osc1Level
                    onLevelChanged: BackEnd.params.osc1Level = level
                    type: BackEnd.params.osc1Type
                    onTypeChanged: BackEnd.params.osc1Type = type
                }
            }

            LabeledPane {
                id: labeledPaneOsc1Wavetable
                title: qsTr("Oscillator 1 Wavetable")
                Wavetable {
                    id: osc1wavetable
                    wavetable: BackEnd.params.osc1Wavetable
                    onWavetableSelected: (v) => { BackEnd.params.osc1Wavetable = v };
                    scanFreq: BackEnd.params.osc1ScanFreq
                    onScanFreqChanged: BackEnd.params.osc1ScanFreq = scanFreq
                    scanPos: BackEnd.params.osc1ScanPos
                    onScanPosChanged: BackEnd.params.osc1ScanPos = scanPos
                    scanAmount: BackEnd.params.osc1ScanAmount
                    onScanAmountChanged: BackEnd.params.osc1ScanAmount = scanAmount
                }
            }

            LabeledPane {
                id: labeledPaneOsc2
                title: qsTr("Oscillator 2")
                Oscillator2 {
                    id: osc2
                    level: BackEnd.params.osc2Level
                    onLevelChanged: BackEnd.params.osc2Level = level
                    type: BackEnd.params.osc2Type
                    onTypeChanged: BackEnd.params.osc2Type = type
                    beat: BackEnd.params.osc2Beat
                    onBeatChanged: BackEnd.params.osc2Beat = beat
                    noiseLevel: BackEnd.params.noiseLevel
                    onNoiseLevelChanged: BackEnd.params.noiseLevel = noiseLevel
                }
            }

            LabeledPane {
                id: labeledPaneOsc2Wavetable
                title: qsTr("Oscillator 2 Wavetable")
                Wavetable {
                    id: osc2wavetable
                    wavetable: BackEnd.params.osc2Wavetable
                    onWavetableSelected: (v) => { BackEnd.params.osc2Wavetable = v };
                    scanFreq: BackEnd.params.osc2ScanFreq
                    onScanFreqChanged: BackEnd.params.osc2ScanFreq = scanFreq
                    scanPos: BackEnd.params.osc2ScanPos
                    onScanPosChanged: BackEnd.params.osc2ScanPos = scanPos
                    scanAmount: BackEnd.params.osc2ScanAmount
                    onScanAmountChanged: BackEnd.params.osc2ScanAmount = scanAmount
                }
            }

            LabeledPane {
                id: labeledPaneOscillator2Filter
                title: qsTr("Oscillator 2 Filter")
                GridLayout {
                    rows: 2
                    columns: 2
                    columnSpacing: 34

                    LabeledSlider {
                        id: sliderPitchOsc2FilterFreq
                        description: "Filter Offset"
                        value: BackEnd.params.osc2FilterFreq
                        unit: "Hz"
                        function renderValue() {
                            let trigger_binding = value; // dummy
                            let hz = BackEnd.params.getOsc2FilterFreqHz();
                            if (Math.abs(hz) < 999.5) {
                                return Number(hz).toFixed(0);
                            } else {
                                return Number(hz / 1000.0).toFixed(1);
                            }
                        }
                        function renderUnit() {
                            let trigger_binding = value; // dummy
                            let hz = BackEnd.params.getOsc2FilterFreqHz();
                            if (Math.abs(hz) < 999.5) {
                                return "Hz";
                            } else {
                                return "kHz";
                            }
                        }
                        onValueChanged: BackEnd.params.osc2FilterFreq = value
                    }

                    Osc2FilterMode {
                        id: osc2FilterModeSelector
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        value: BackEnd.params.osc2FilterMode
                        // custom signal to avoid breaking bindings:
                        onValueSelected: (v) => { BackEnd.params.osc2FilterMode = v };
                    }

                    LabeledSlider {
                        id: sliderPitchOsc2FilterRes
                        description: "Resonance"
                        value: BackEnd.params.osc2FilterRes
                        onValueChanged: BackEnd.params.osc2FilterRes = value
                        unit: "%"
                        function renderValue() {
                            return Number(_interpolate(value, from, to, 0, 100)).toFixed(0);
                        }
                    }

                    Osc2FilterEnable {
                        id: osc2FilterEnableSelector
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        value: BackEnd.params.osc2FilterEnable
                        // custom signal to avoid breaking bindings:
                        onValueSelected: (v) => { BackEnd.params.osc2FilterEnable = v };
                    }
                }
            }
            RowLayout {
                spacing: flow1.spacing
                LabeledPane {
                    id: labeledPanePitchFilter
                    title: qsTr("Pitch Filter Tracking")
                    GridLayout {
                        rows: 2
                        columnSpacing: 34
                        columns: 1

                        LabeledSlider {
                            id: sliderPitchFilterOsc1
                            Layout.columnSpan: 1
                            value: BackEnd.params.osc1FilterPitchAmount
                            onValueChanged: BackEnd.params.osc1FilterPitchAmount = value
                            unit: "%"
                            description: "Osc 1"
                            function renderValue() {
                                return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                            }
                        }

                        LabeledSlider {
                            id: sliderPitchFilterOsc2
                            Layout.columnSpan: 1
                            value: BackEnd.params.osc2FilterPitchAmount
                            onValueChanged: BackEnd.params.osc2FilterPitchAmount = value
                            unit: "%"
                            description: "Osc 2"
                            function renderValue() {
                                return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                            }
                        }
                    }
                }

                LabeledPane {
                    id: labeledPaneVolFilter
                    title: qsTr("Volume Filter Tracking")
                    GridLayout {
                        rows: 2
                        columnSpacing: 34
                        columns: 1

                        LabeledSlider {
                            id: sliderVolFilterOsc1
                            Layout.columnSpan: 1
                            value: BackEnd.params.osc1FilterVolumeAmount
                            onValueChanged: BackEnd.params.osc1FilterVolumeAmount = value
                            unit: "%"
                            description: "Osc 1"
                            function renderValue() {
                                return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                            }
                        }

                        LabeledSlider {
                            id: sliderVolFilterOsc2
                            Layout.columnSpan: 1
                            value: BackEnd.params.osc2FilterVolumeAmount
                            onValueChanged: BackEnd.params.osc2FilterVolumeAmount = value
                            unit: "%"
                            description: "Osc 2"
                            function renderValue() {
                                return Number(_interpolate(value, from, to, -100, 100)).toFixed(0);
                            }
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
