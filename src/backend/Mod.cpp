#include "Mod.h"
#include "Util.h"
#include <QJsonObject>

Mod::Mod(QObject *parent)
    : QObject{parent}
{

}

bool Mod::parse(const QJsonObject& obj) {
    bool ok =
            obj.value("cc").isDouble() &&
            obj.value("pitchCvEnable").isBool() &&
            obj.value("pitchCvAmount").isDouble() &&
            obj.value("volumeCvEnable").isBool() &&
            obj.value("volumeCvAmount").isDouble() &&
            obj.value("externalCvEnable").isBool() &&
            obj.value("externalCvAmount").isDouble();
    if (!ok)
        return false;
    m_cc = obj.value("cc").toInt();
    m_pitchCvEnable = obj.value("pitchCvEnable").toBool();
    m_pitchCvAmount = obj.value("pitchCvAmount").toDouble();
    m_volumeCvEnable = obj.value("volumeCvEnable").toBool();
    m_volumeCvAmount = obj.value("volumeCvAmount").toDouble();
    m_externalCvEnable = obj.value("externalCvEnable").toBool();
    m_externalCvAmount = obj.value("externalCvAmount").toDouble();
    return ok && isValid();
}

bool Mod::isValid() const {
    return in_range(m_cc, 0, 16383) &&
            in_range(m_pitchCvAmount, 0.0, 1.0) &&
            in_range(m_volumeCvAmount, 0.0, 1.0) &&
            in_range(m_externalCvAmount, 0.0, 1.0);
}

QJsonObject Mod::serialize() const {
    QJsonObject out;
    out.insert("cc", m_cc);
    out.insert("pitchCvEnable", m_pitchCvEnable);
    out.insert("pitchCvAmount", m_pitchCvAmount);
    out.insert("volumeCvEnable", m_volumeCvEnable);
    out.insert("volumeCvAmount", m_volumeCvAmount);
    out.insert("externalCvEnable", m_externalCvEnable);
    out.insert("externalCvAmount", m_externalCvAmount);
    return out;
}

bool Mod::enabled() {
    return m_pitchCvEnable || m_volumeCvEnable || m_externalCvEnable;
}
