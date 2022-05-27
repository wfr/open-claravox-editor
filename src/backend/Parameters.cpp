// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "Parameters.h"
#include "Util.h"
#include <cmath>
#include <QList>
#include <QException>
#include <QEvent>
#include <QMetaProperty>

Parameters::Parameters(QObject *parent) : QObject(parent)
{
    // Connect to all properties (not sure how...)
//    qDebug() << "Parameters properties:";
//    const QMetaObject* metaObj = this->metaObject();
//    for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); i++) {
//        QMetaProperty metaProp = metaObj->property(i);
//    }

    // Please let me know if you know how to connect QMetaMethod to a slot.
    // Until then, this will have to do.
    connect(this, &Parameters::brightValChanged, this, [this]() { emitPropertyChanged("brightVal"); });
    connect(this, &Parameters::delayAmountChanged, this, [this]() { emitPropertyChanged("delayAmount"); });
    connect(this, &Parameters::feedbackValChanged, this, [this]() { emitPropertyChanged("feedbackVal"); });
    connect(this, &Parameters::filterValChanged, this, [this]() { emitPropertyChanged("filterVal"); });
    connect(this, &Parameters::noiseLevelChanged, this, [this]() { emitPropertyChanged("noiseLevel"); });
    connect(this, &Parameters::octaveStateChanged, this, [this]() { emitPropertyChanged("octaveState"); });
    connect(this, &Parameters::osc1FilterPitchAmountChanged, this, [this]() { emitPropertyChanged("osc1FilterPitchAmount"); });
    connect(this, &Parameters::osc1FilterVolumeAmountChanged, this, [this]() { emitPropertyChanged("osc1FilterVolumeAmount"); });
    connect(this, &Parameters::osc1LevelChanged, this, [this]() { emitPropertyChanged("osc1Level"); });
    connect(this, &Parameters::osc1ScanAmountChanged, this, [this]() { emitPropertyChanged("osc1ScanAmount"); });
    connect(this, &Parameters::osc1ScanFreqChanged, this, [this]() { emitPropertyChanged("osc1ScanFreq"); });
    connect(this, &Parameters::osc1ScanPosChanged, this, [this]() { emitPropertyChanged("osc1ScanPos"); });
    connect(this, &Parameters::osc1TypeChanged, this, [this]() { emitPropertyChanged("osc1Type"); });
    connect(this, &Parameters::osc1WavetableChanged, this, [this]() { emitPropertyChanged("osc1Wavetable"); });
    connect(this, &Parameters::osc2BeatChanged, this, [this]() { emitPropertyChanged("osc2Beat"); });
    connect(this, &Parameters::osc2FilterEnableChanged, this, [this]() { emitPropertyChanged("osc2FilterEnable"); });
    connect(this, &Parameters::osc2FilterFreqChanged, this, [this]() { emitPropertyChanged("osc2FilterFreq"); });
    connect(this, &Parameters::osc2FilterModeChanged, this, [this]() { emitPropertyChanged("osc2FilterMode"); });
    connect(this, &Parameters::osc2FilterPitchAmountChanged, this, [this]() { emitPropertyChanged("osc2FilterPitchAmount"); });
    connect(this, &Parameters::osc2FilterResChanged, this, [this]() { emitPropertyChanged("osc2FilterRes"); });
    connect(this, &Parameters::osc2FilterVolumeAmountChanged, this, [this]() { emitPropertyChanged("osc2FilterVolumeAmount"); });
    connect(this, &Parameters::osc2FreqChanged, this, [this]() { emitPropertyChanged("osc2Freq"); });
    connect(this, &Parameters::osc2LevelChanged, this, [this]() { emitPropertyChanged("osc2Level"); });
    connect(this, &Parameters::osc2ScanAmountChanged, this, [this]() { emitPropertyChanged("osc2ScanAmount"); });
    connect(this, &Parameters::osc2ScanFreqChanged, this, [this]() { emitPropertyChanged("osc2ScanFreq"); });
    connect(this, &Parameters::osc2ScanPosChanged, this, [this]() { emitPropertyChanged("osc2ScanPos"); });
    connect(this, &Parameters::osc2TypeChanged, this, [this]() { emitPropertyChanged("osc2Type"); });
    connect(this, &Parameters::osc2WavetableChanged, this, [this]() { emitPropertyChanged("osc2Wavetable"); });
    connect(this, &Parameters::pitchBendRangeChanged, this, [this]() { emitPropertyChanged("pitchBendRange"); });
    connect(this, &Parameters::pitchCurveValChanged, this, [this]() { emitPropertyChanged("pitchCurveVal"); });
    connect(this, &Parameters::pitchModWsAmountChanged, this, [this]() { emitPropertyChanged("pitchModWsAmount"); });
    connect(this, &Parameters::pitchModWsFreqChanged, this, [this]() { emitPropertyChanged("pitchModWsFreq"); });
    connect(this, &Parameters::quantizeAmountChanged, this, [this]() { emitPropertyChanged("quantizeAmount"); });
    connect(this, &Parameters::rootValChanged, this, [this]() { emitPropertyChanged("rootVal"); });
    connect(this, &Parameters::scaleValChanged, this, [this]() { emitPropertyChanged("scaleVal"); });
    connect(this, &Parameters::timeValChanged, this, [this]() { emitPropertyChanged("timeVal"); });
    connect(this, &Parameters::volCurveValChanged, this, [this]() { emitPropertyChanged("volCurveVal"); });
    connect(this, &Parameters::waveValChanged, this, [this]() { emitPropertyChanged("waveVal"); });
}

void Parameters::emitPropertyChanged(const QString& property_name) {
//    qDebug() << "Parameters::onPropertyChanged(" << property_name << ")";
    emit propertyChanged(property_name);
}

void Parameters::assign(const Parameters& other) {
    auto mo = this->metaObject();
    for (int i = mo->propertyOffset(); i < mo->propertyCount(); i++) {
        QMetaProperty mp = mo->property(i);
        setProperty(mp.name(), mp.read(&other));
    }
}

bool Parameters::parse(const QJsonObject& obj) {
    bool ok = true;
    ok &= getJsonValue(obj, "brightVal", m_brightVal);
    ok &= getJsonValue(obj, "delayAmount", m_delayAmount);
    ok &= getJsonValue(obj, "feedbackVal", m_feedbackVal);
    ok &= getJsonValue(obj, "filterVal", m_filterVal);
    ok &= getJsonValue(obj, "knobScale1", m_knobScale1);
    ok &= getJsonValue(obj, "knobScale2", m_knobScale2);
    ok &= getJsonValue(obj, "knobScale3", m_knobScale3);
    ok &= getJsonValue(obj, "knobScale4", m_knobScale4);
    ok &= getJsonValue(obj, "midiNoteNumber", m_midiNoteNumber);
    ok &= getJsonValue(obj, "midiNoteNumberMode", m_midiNoteNumberMode);
    ok &= getJsonValue(obj, "midiPitchOut", m_midiPitchOut);
    ok &= getJsonValue(obj, "noiseLevel", m_noiseLevel);
    ok &= getJsonValue(obj, "octaveState", m_octaveState);
    ok &= getJsonValue(obj, "osc1FilterPitchAmount", m_osc1FilterPitchAmount);
    ok &= getJsonValue(obj, "osc1FilterVolumeAmount", m_osc1FilterVolumeAmount);
    ok &= getJsonValue(obj, "osc1Level", m_osc1Level);
    ok &= getJsonValue(obj, "osc1ScanAmount", m_osc1ScanAmount);
    ok &= getJsonValue(obj, "osc1ScanFreq", m_osc1ScanFreq);
    ok &= getJsonValue(obj, "osc1ScanPos", m_osc1ScanPos);
    ok &= getJsonValue(obj, "osc1Type", m_osc1Type);
    ok &= getJsonValue(obj, "osc1Wavetable", m_osc1Wavetable);
    ok &= getJsonValue(obj, "osc2Beat", m_osc2Beat);
    ok &= getJsonValue(obj, "osc2FilterEnable", m_osc2FilterEnable);
    ok &= getJsonValue(obj, "osc2FilterFreq", m_osc2FilterFreq);
    ok &= getJsonValue(obj, "osc2FilterMode", m_osc2FilterMode);
    ok &= getJsonValue(obj, "osc2FilterPitchAmount", m_osc2FilterPitchAmount);
    ok &= getJsonValue(obj, "osc2FilterRes", m_osc2FilterRes);
    ok &= getJsonValue(obj, "osc2FilterVolumeAmount", m_osc2FilterVolumeAmount);
    ok &= getJsonValue(obj, "osc2Freq", m_osc2Freq);
    ok &= getJsonValue(obj, "osc2Level", m_osc2Level);
    ok &= getJsonValue(obj, "osc2ScanAmount", m_osc2ScanAmount);
    ok &= getJsonValue(obj, "osc2ScanFreq", m_osc2ScanFreq);
    ok &= getJsonValue(obj, "osc2ScanPos", m_osc2ScanPos);
    ok &= getJsonValue(obj, "osc2Type", m_osc2Type);
    ok &= getJsonValue(obj, "osc2Wavetable", m_osc2Wavetable);
    ok &= getJsonValue(obj, "pitchBendRange", m_pitchBendRange);
    ok &= getJsonValue(obj, "pitchCurveVal", m_pitchCurveVal);
    ok &= getJsonValue(obj, "pitchModWsAmount", m_pitchModWsAmount);
    ok &= getJsonValue(obj, "pitchModWsFreq", m_pitchModWsFreq);
    ok &= getJsonValue(obj, "quantizeAmount", m_quantizeAmount);
    ok &= getJsonValue(obj, "rootVal", m_rootVal);
    ok &= getJsonValue(obj, "scaleVal", m_scaleVal);
    ok &= getJsonValue(obj, "timeVal", m_timeVal);
    ok &= getJsonValue(obj, "volCurveVal", m_volCurveVal);
    ok &= getJsonValue(obj, "waveVal", m_waveVal);
    return ok;
}

QJsonObject Parameters::serialize() const {
    QJsonObject result;
    auto metaobj = this->metaObject();
    for (int i = metaobj->propertyOffset(); i < metaobj->propertyCount(); i++) {
        QMetaProperty metaprop = metaobj->property(i);
        result.insert(metaprop.name(), metaprop.read(this).toDouble());
    }
    return result;
}

double Parameters::filterValToHz(double x) {
    // Moog's mapping from the filter's internal representation to Hertz is undocumented.
    // In order to discover an approximation, I
    // - observed raw values [0..1] an and displayed values [100..8000]
    // - chose a function that seems reasonable
    // - applied a curve fitting algorithm to determine the coefficients.

    // f(x) = a * e^(b * x) + c
    double a = 100.097988300907474013001774437725543975830078125;
    double b = 4.38097322962329993600860689184628427028656005859375;
    double c = -0.57183090249984569464203332245233468711376190185546875;
    double hz = a * exp(b * x) + c;

    // The mapping is slightly inaccurate at the boundaries.
    // Let's cheat a little:
    if (hz < 100) {
        hz = 100;
    }
    if (hz > 7998) {
        hz = 8000;
    }
    return hz;
}

double Parameters::oscScanFreqToHz(double x) {
    // f(x) = a * e^(b * x) + c
    double a = 0.100228445855058231894751941126742167398333549499511718750000;
    double b = 7.595550931749315282104362268000841140747070312500000000000000;
    double c = 0.005282335106109882147396028528874012408778071403503417968750;
    double hz = a * exp(b * x) + c;
    return hz;
}

double Parameters::oscFilterFreqToHz(double x) {
    // This approximation is too inaccurate.
    // f(x) = sgn(x - 0.5) * a * exp(b * |(x - 0.5)|) + c
    double a = 0.944099966907212317934749989944975823163986206054687500000000;
    double b = 18.093246362123018400325236143544316291809082031250000000000000;
    double c = -0.019607053021198462411378571346176613587886095046997070312500;
    double hz = copysign(1.0, x - 0.5) * a * exp(b * abs(x - 0.5)) + c;

    // Cheat
    hz = hz * 0.993 - 0.7;
    if (hz < -8000.0) {
        hz = -8000.0;
    } else if (hz > 8000.0) {
        hz = 8000.0;
    }

    return hz;
}

double Parameters::getFilterValHz() const {
    return filterValToHz(m_filterVal);
}

double Parameters::getOsc1ScanFreqHz() const  {
    return oscScanFreqToHz(m_osc1ScanFreq);
}

double Parameters::getOsc2ScanFreqHz() const  {
    return oscScanFreqToHz(m_osc2ScanFreq);
}

double Parameters::getOsc2FilterFreqHz() const  {
    return oscFilterFreqToHz(m_osc2FilterFreq);
}

const QList<QString>& Parameters::scaleNames() const {
    static const QList<QString> scale_names = {
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
        "Min3rd",
    };
    return scale_names;
}

const QString& Parameters::scaleName(double val) const {
    val = std::min(1.0, std::max(0.0, val));
    return scaleNames().at(int(val * scaleNames().count()));
}

const QString& Parameters::scaleName() const {
    return scaleName(property("scaleVal").toDouble());
}

void Parameters::setScale(const QString& name) {
    static QMap<QString, double> scale_to_value_map;
    if (scale_to_value_map.count() == 0) {
        for (int i = 0; i < scaleNames().count(); i++) {
            double v = double(i) / scaleNames().count();
            scale_to_value_map[scaleNames()[i]] = v;
        }
    }

    assert(scale_to_value_map.contains(name));
    setProperty("scaleVal", scale_to_value_map[name]);
}
