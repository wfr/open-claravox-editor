// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QObject>
#include "Mod.h"

/**
 * @brief The ModMatrix class represents a set of CV mods.
 *
 * Each preset contains a mod matrix with 34 slots, one for each of the 34 modifiable parameters.
 * 10 modifier slots may be used simultaneously.
 *
 * Each slot refers to a specific parameter that is modified by up to 3 CV sources.
 * See ModMatrix.cpp for details.
 */
class ModMatrix : public QObject
{
    Q_OBJECT
public:
    static const size_t MAX_MODS = 34;
    static const size_t MAX_SIMULTANEOUS_MODS = 10;

    explicit ModMatrix(QObject *parent = nullptr);

    size_t countMods();
    bool isValid();
    void removeUnusedMods();

    bool parse(const QJsonObject& obj);
    QJsonObject serialize();

signals:
    void changed();

protected:
    std::array<Mod*, MAX_MODS> m_mods;
};

