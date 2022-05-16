#include "Mod.h"
#include <QJsonObject>

Mod::Mod(QObject *parent)
    : QObject{parent}
{

}

void Mod::parse(const QJsonObject& obj) {
    m_cc = obj.value("cc").toInt();
    m_pitchCvEnable = obj.value("pitchCvEnable").toBool();
    m_pitchCvAmount = obj.value("pitchCvAmount").toDouble();
    m_volumeCvEnable = obj.value("volumeCvEnable").toBool();
    m_volumeCvAmount = obj.value("volumeCvAmount").toDouble();
    m_externalCvEnable = obj.value("externalCvEnable").toBool();
    m_externalCvAmount = obj.value("externalCvAmount").toDouble();
}

QJsonObject Mod::serialize() {
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
