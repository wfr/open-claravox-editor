// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "Preset.h"
#include <cassert>
#include <QJsonArray>

Preset::Preset(QObject *parent) : QObject(parent)
{

}

Preset::Preset(QObject *parent, const QJsonObject& obj)
    : QObject(parent) {
    // TODO error handling
    m_instrument = obj.value("instrument").toString();
    m_name = obj.value("name").toString();
    m_group = obj.value("group").toString();
    for (const auto value : obj.value("tags").toArray()) {
        m_tags.push_back(value.toString());
    }
    m_parameters = new Parameters(this, obj.value("parameters").toObject());
    m_taglistmodel = new QStringListModel(QStringList(m_tags), this);
}

QJsonObject Preset::serialize() {
    QJsonObject result;
    result["instrument"] = m_instrument;
    result["name"] = m_name;
    result["group"] = m_group;
    result["tags"] = QJsonArray();
    for (const auto& tag : m_tags) {
        result["tags"].toArray().append(QJsonValue(tag));
    }
    result["parameters"] = m_parameters->serialize();
    return result;
}

Preset* Preset::clone() {
    return new Preset(parent(), serialize());
}
