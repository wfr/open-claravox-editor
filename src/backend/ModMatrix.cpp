// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "ModMatrix.h"
#include <QJsonArray>
#include <QJsonObject>

struct ModMeta {
    size_t pos;       // position in the modMatrix
    uint8_t cc;       // corresponding CC
    const char* name; // parameter name (internal)
};

ModMatrix::ModMatrix(QObject *parent)
    : QObject{parent}
{
    for (size_t i = 0; i < MAX_MODS; i++) {
        m_mods.at(i) = nullptr;
    }

    static const std::vector<ModMeta> modmeta_list = {
        {  0, 12, "volCurveVal" },
        {  1, 13, "pitchCurveVal" },
        {  2, 16, "quantizeAmount" },
        {  3, 85, "rootVal" },
        {  4, 81, "osc1Type" },
        {  5, 25, "osc1Level" },
        {  6, 83, "osc1Wavetable" },
        {  7, 23, "osc1ScanFreq" },
        {  8, 87, "osc1ScanPos" },
        {  9, 89, "osc1ScanAmount" },
        { 10, 80, "osc2Type" },
        { 11, 26, "osc2Level" },
        /* Position 12 can modulate either:
           - CC 27, Oscillator 2 Semitone Offset (osc2Freq)
           - CC 28, Oscillator 2 Beat Frequency (osc2Beat)
        */
        { 12, 27, "osc2Freq" },
        { 12, 28, "osc2Beat" },
        { 13,  9, "noiseLevel" }, // osc2
        { 14, 82, "osc2Wavetable" },
        { 15, 24, "osc2ScanFreq" },
        { 16, 88, "osc2ScanPos" },
        { 17, 90, "osc2ScanAmount" },
        { 18,  8, "osc2FilterFreq" },
        { 19, 10, "osc2FilterRes" },
        { 20, 91, "osc2FilterMode" },
        { 21,  4, "osc1FilterVolumeAmount" },
        { 22,  5, "osc2FilterVolumeAmount" },
        { 23,  2, "osc1FilterPitchAmount" },
        { 24,  3, "osc2FilterPitchAmount" },
        { 25, 72, "pitchModWsFreq" },   // Wavetable Scan Freq
        { 26, 73, "pitchModWsAmount" }, // Wavetable Scan Pos
        { 27, 75, "octaveState" },      // Register
        { 28, 19, "filterVal" },        // Filter Cutoff
        { 29, 17, "brightVal" },
        { 30, 18, "waveVal" },
        { 31, 20, "timeVal" },          // Delay Time
        { 32, 22, "feedbackVal" },      // Delay Feedback
        { 33, 21, "delayAmount" }       // Delay Mix
    };

    static std::map<uint8_t, ModMeta> cc_modmeta;
    if (cc_modmeta.size() == 0) {
        for (const ModMeta& mm : modmeta_list) {
            cc_modmeta[mm.cc] = mm;
        }
    }
}

size_t ModMatrix::countMods() {
    size_t cnt = 0;
    for (size_t i = 0; i < m_mods.size(); i++) {
        if (m_mods.at(i) != nullptr) {
            cnt++;
        }
    }
    return cnt;
}

bool ModMatrix::isValid() {
    return countMods() <= MAX_SIMULTANEOUS_MODS;
}

void ModMatrix::removeUnusedMods() {
    for (size_t i = 0; i < MAX_MODS; i++) {
        if (m_mods.at(i) != nullptr && !m_mods.at(i)->enabled()) {
            m_mods.at(i) = nullptr;
        }
    }
}

bool ModMatrix::parse(const QJsonObject& obj) {
    bool ok = true;
    QJsonArray destinations = obj.value("destinations").toArray();
    if (destinations.size() != MAX_MODS) {
        qWarning() << "ModMatrix::parse destinations.size() != MAX_MODS";
        return false;
    }
    for (size_t pos = 0; pos < MAX_MODS; pos++) {
        if (destinations.at(pos).isNull()) {
            m_mods.at(pos) = nullptr;
        } else {
            Mod* m = new Mod(this);
            if (!m->parse(destinations.at(pos).toObject())) {
                qWarning() << "ModMatrix::parse failed at pos" << pos;
            }
            m_mods.at(pos) = m;
        }
    }
    removeUnusedMods();
    return ok && isValid();
}

QJsonObject ModMatrix::serialize() {
    QJsonObject out;
    QJsonArray destinations;
    for (size_t pos = 0; pos < MAX_MODS; pos++) {
        if (m_mods.at(pos) == nullptr) {
            destinations.append(QJsonValue());
        } else {
            destinations.append(m_mods.at(pos)->serialize());
        }
    }
    out["destinations"] = destinations;
    return out;
}
