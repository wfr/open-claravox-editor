#pragma once

#include <QObject>
#include "Mod.h"


class ModMatrix : public QObject
{
    Q_OBJECT
public:
    static const size_t MAX_MODS = 34;
    static const size_t MAX_SIMULTANEOUS_MODS = 10;

    explicit ModMatrix(QObject *parent = nullptr);
    ModMatrix(QObject* parent, const QJsonObject& obj);
    size_t countMods();
    bool isValid();
    void cleanup(); // remove unused mods

    void parse(const QJsonObject& obj);
    QJsonObject serialize();

signals:
    void changed();

protected:
    std::array<Mod*, MAX_MODS> m_mods;
};

