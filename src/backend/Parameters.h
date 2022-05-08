// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QObject>
#include <QJsonObject>

/*!
 * \brief The Parameters class
 *
 * Encodes all parameters of a Claravox preset.
 *
 * We could make this less verbose by generating the properties dynamically at run-time,
 * but since they're very unlikely to ever change, a verbose hardcoded class is acceptable.
 */
class Parameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double brightVal              MEMBER m_brightVal NOTIFY brightValChanged)
    Q_PROPERTY(double delayAmount            MEMBER m_delayAmount NOTIFY delayAmountChanged)
    Q_PROPERTY(double feedbackVal            MEMBER m_feedbackVal NOTIFY feedbackValChanged)
    Q_PROPERTY(double filterVal              MEMBER m_filterVal NOTIFY filterValChanged)
    Q_PROPERTY(double knobScale1             MEMBER m_knobScale1)
    Q_PROPERTY(double knobScale2             MEMBER m_knobScale2)
    Q_PROPERTY(double knobScale3             MEMBER m_knobScale3)
    Q_PROPERTY(double knobScale4             MEMBER m_knobScale4)
    Q_PROPERTY(double midiNoteNumber         MEMBER m_midiNoteNumber)
    Q_PROPERTY(double midiNoteNumberMode     MEMBER m_midiNoteNumberMode)
    Q_PROPERTY(double midiPitchOut           MEMBER m_midiPitchOut)
    Q_PROPERTY(double noiseLevel             MEMBER m_noiseLevel NOTIFY noiseLevelChanged)
    Q_PROPERTY(double octaveState            MEMBER m_octaveState NOTIFY octaveStateChanged)
    Q_PROPERTY(double osc1FilterPitchAmount  MEMBER m_osc1FilterPitchAmount NOTIFY osc1FilterPitchAmountChanged)
    Q_PROPERTY(double osc1FilterVolumeAmount MEMBER m_osc1FilterVolumeAmount NOTIFY osc1FilterVolumeAmountChanged)
    Q_PROPERTY(double osc1Level              MEMBER m_osc1Level NOTIFY osc1LevelChanged)
    Q_PROPERTY(double osc1ScanAmount         MEMBER m_osc1ScanAmount NOTIFY osc1ScanAmountChanged)
    Q_PROPERTY(double osc1ScanFreq           MEMBER m_osc1ScanFreq NOTIFY osc1ScanFreqChanged)
    Q_PROPERTY(double osc1ScanPos            MEMBER m_osc1ScanPos NOTIFY osc1ScanPosChanged)
    Q_PROPERTY(double osc1Type               MEMBER m_osc1Type NOTIFY osc1TypeChanged)
    Q_PROPERTY(double osc1Wavetable          MEMBER m_osc1Wavetable NOTIFY osc1WavetableChanged)
    Q_PROPERTY(double osc2Beat               MEMBER m_osc2Beat NOTIFY osc2BeatChanged)
    Q_PROPERTY(double osc2FilterEnable       MEMBER m_osc2FilterEnable NOTIFY osc2FilterEnableChanged)
    Q_PROPERTY(double osc2FilterFreq         MEMBER m_osc2FilterFreq NOTIFY osc2FilterFreqChanged)
    Q_PROPERTY(double osc2FilterMode         MEMBER m_osc2FilterMode NOTIFY osc2FilterModeChanged)
    Q_PROPERTY(double osc2FilterPitchAmount  MEMBER m_osc2FilterPitchAmount NOTIFY osc2FilterPitchAmountChanged)
    Q_PROPERTY(double osc2FilterRes          MEMBER m_osc2FilterRes NOTIFY osc2FilterResChanged)
    Q_PROPERTY(double osc2FilterVolumeAmount MEMBER m_osc2FilterVolumeAmount NOTIFY osc2FilterVolumeAmountChanged)
    Q_PROPERTY(double osc2Freq               MEMBER m_osc2Freq NOTIFY osc2FreqChanged)
    Q_PROPERTY(double osc2Level              MEMBER m_osc2Level NOTIFY osc2LevelChanged)
    Q_PROPERTY(double osc2ScanAmount         MEMBER m_osc2ScanAmount NOTIFY osc2ScanAmountChanged)
    Q_PROPERTY(double osc2ScanFreq           MEMBER m_osc2ScanFreq NOTIFY osc2ScanFreqChanged)
    Q_PROPERTY(double osc2ScanPos            MEMBER m_osc2ScanPos NOTIFY osc2ScanPosChanged)
    Q_PROPERTY(double osc2Type               MEMBER m_osc2Type NOTIFY osc2TypeChanged)
    Q_PROPERTY(double osc2Wavetable          MEMBER m_osc2Wavetable NOTIFY osc2WavetableChanged)
    Q_PROPERTY(double pitchBendRange         MEMBER m_pitchBendRange NOTIFY pitchBendRangeChanged)
    Q_PROPERTY(double pitchCurveVal          MEMBER m_pitchCurveVal NOTIFY pitchCurveValChanged)
    Q_PROPERTY(double pitchModWsAmount       MEMBER m_pitchModWsAmount NOTIFY pitchModWsAmountChanged) // Pitch Antenna Mod: Wavetable Scan Pos
    Q_PROPERTY(double pitchModWsFreq         MEMBER m_pitchModWsFreq NOTIFY pitchModWsFreqChanged)   // Pitch Antenna Mod: Wavetable Scan Freq
    Q_PROPERTY(double quantizeAmount         MEMBER m_quantizeAmount NOTIFY quantizeAmountChanged)
    Q_PROPERTY(double rootVal                MEMBER m_rootVal NOTIFY rootValChanged)
    Q_PROPERTY(double scaleVal               MEMBER m_scaleVal NOTIFY scaleValChanged)
    Q_PROPERTY(double timeVal                MEMBER m_timeVal NOTIFY timeValChanged)
    Q_PROPERTY(double volCurveVal            MEMBER m_volCurveVal NOTIFY volCurveValChanged)
    Q_PROPERTY(double waveVal                MEMBER m_waveVal NOTIFY waveValChanged)

public:
    explicit Parameters(QObject *parent = nullptr);
    Parameters(QObject *parent, const QJsonObject& obj);
    void assign(const Parameters& other);
    QJsonObject serialize();

    Q_INVOKABLE static double filterValToHz(double x);
    Q_INVOKABLE static double oscScanFreqToHz(double x);
    Q_INVOKABLE static double oscFilterFreqToHz(double x);
    Q_INVOKABLE double getFilterValHz() const;
    Q_INVOKABLE double getOsc1ScanFreqHz() const;
    Q_INVOKABLE double getOsc2ScanFreqHz() const;
    Q_INVOKABLE double getOsc2FilterFreqHz() const;
    Q_INVOKABLE const QList<QString>& scaleNames() const;
    Q_INVOKABLE const QString& scaleName(double val) const;
    Q_INVOKABLE const QString& scaleName() const;
    Q_INVOKABLE void setScale(const QString& name);

signals:
    // Regex to generate the signals in Vim:
    // %s/.*NOTIFY \([a-zA-Z0-9]\+\))/void \1\(double val\);/
    void brightValChanged(double val);
    void delayAmountChanged(double val);
    void feedbackValChanged(double val);
    void filterValChanged(double val);
    void noiseLevelChanged(double val);
    void octaveStateChanged(double val);
    void osc1FilterPitchAmountChanged(double val);
    void osc1FilterVolumeAmountChanged(double val);
    void osc1LevelChanged(double val);
    void osc1ScanAmountChanged(double val);
    void osc1ScanFreqChanged(double val);
    void osc1ScanPosChanged(double val);
    void osc1TypeChanged(double val);
    void osc1WavetableChanged(double val);
    void osc2BeatChanged(double val);
    void osc2FilterEnableChanged(double val);
    void osc2FilterFreqChanged(double val);
    void osc2FilterModeChanged(double val);
    void osc2FilterPitchAmountChanged(double val);
    void osc2FilterResChanged(double val);
    void osc2FilterVolumeAmountChanged(double val);
    void osc2FreqChanged(double val);
    void osc2LevelChanged(double val);
    void osc2ScanAmountChanged(double val);
    void osc2ScanFreqChanged(double val);
    void osc2ScanPosChanged(double val);
    void osc2TypeChanged(double val);
    void osc2WavetableChanged(double val);
    void pitchBendRangeChanged(double val);
    void pitchCurveValChanged(double val);
    void pitchModWsAmountChanged(double val); // Pitch Antenna Mod: Wavetable Scan Pos
    void pitchModWsFreqChanged(double val);   // Pitch Antenna Mod: Wavetable Scan Freq
    void quantizeAmountChanged(double val);
    void rootValChanged(double val);
    void scaleValChanged(double val);
    void timeValChanged(double val);
    void volCurveValChanged(double val);
    void waveValChanged(double val);

    void propertyChanged(QString property_name);

protected:
    void emitPropertyChanged(const QString& property_name);

private:
    double m_brightVal;
    double m_delayAmount;
    double m_feedbackVal;
    double m_filterVal;
    double m_knobScale1;
    double m_knobScale2;
    double m_knobScale3;
    double m_knobScale4;
    double m_midiNoteNumber;
    double m_midiNoteNumberMode;
    double m_midiPitchOut;
    double m_noiseLevel;
    double m_octaveState;
    double m_osc1FilterPitchAmount;
    double m_osc1FilterVolumeAmount;
    double m_osc1Level;
    double m_osc1ScanAmount;
    double m_osc1ScanFreq;
    double m_osc1ScanPos;
    double m_osc1Type;
    double m_osc1Wavetable;
    double m_osc2Beat;
    double m_osc2FilterEnable;
    double m_osc2FilterFreq;
    double m_osc2FilterMode;
    double m_osc2FilterPitchAmount;
    double m_osc2FilterRes;
    double m_osc2FilterVolumeAmount;
    double m_osc2Freq;
    double m_osc2Level;
    double m_osc2ScanAmount;
    double m_osc2ScanFreq;
    double m_osc2ScanPos;
    double m_osc2Type;
    double m_osc2Wavetable;
    double m_pitchBendRange;
    double m_pitchCurveVal;
    double m_pitchModWsAmount;
    double m_pitchModWsFreq;
    double m_quantizeAmount;
    double m_rootVal;
    double m_scaleVal;
    double m_timeVal;
    double m_volCurveVal;
    double m_waveVal;
};
