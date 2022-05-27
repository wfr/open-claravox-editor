// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "Preset.h"
#include "Util.h"
#include <cassert>
#include <QJsonArray>

Preset::Preset(QObject *parent) : QObject(parent)
{
    m_parameters = new Parameters(this);
    m_taglistmodel = new QStringListModel(this);
    m_modMatrix = new ModMatrix(this);
}

bool Preset::parse(const QJsonObject& obj) {
    bool ok = true;
    ok &= getJsonValue(obj, "instrument", m_instrument);
    ok &= getJsonValue(obj, "name", m_name);
    ok &= getJsonValue(obj, "group", m_group);
    ok &= m_parameters->parse(obj.value("parameters").toObject());
    for (const auto value : obj.value("tags").toArray()) {
        m_tags.push_back(value.toString());
    }
    m_taglistmodel->setStringList(m_tags);
    ok &= m_modMatrix->parse(obj.value("modMatrix").toObject());
    return ok;
}

QJsonObject Preset::serialize() const {
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
    auto p = new Preset(parent());
    p->parse(serialize());
    return p;
}
