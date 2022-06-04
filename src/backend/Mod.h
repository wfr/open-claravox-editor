// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QObject>

class Mod : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int cc MEMBER m_cc NOTIFY ccChanged)
    Q_PROPERTY(bool pitchCvEnable MEMBER m_pitchCvEnable NOTIFY pitchCvEnableChanged)
    Q_PROPERTY(double pitchCvAmount MEMBER m_pitchCvAmount NOTIFY pitchCvAmountChanged)
    Q_PROPERTY(bool volumeCvEnable MEMBER m_volumeCvEnable NOTIFY volumeCvEnableChanged)
    Q_PROPERTY(double volumeCvAmount MEMBER m_volumeCvAmount NOTIFY volumeCvAmountChanged)
    Q_PROPERTY(bool externalCvEnable MEMBER m_externalCvEnable NOTIFY externalCvEnableChanged)
    Q_PROPERTY(double externalCvAmount MEMBER m_externalCvAmount NOTIFY externalCvAmountChanged)
public:
    explicit Mod(QObject *parent = nullptr);

    bool parse(const QJsonObject& obj);
    bool isValid() const;
    QJsonObject serialize() const;

    bool enabled();

signals:
    void ccChanged();
    void pitchCvEnableChanged();
    void pitchCvAmountChanged();
    void volumeCvEnableChanged();
    void volumeCvAmountChanged();
    void externalCvEnableChanged();
    void externalCvAmountChanged();

protected:
    int m_cc;
    bool m_pitchCvEnable;
    double m_pitchCvAmount;
    bool m_volumeCvEnable;
    double m_volumeCvAmount;
    bool m_externalCvEnable;
    double m_externalCvAmount;
};
