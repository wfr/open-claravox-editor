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
    result["version"] = 1;
    result["instrument"] = m_instrument;
    result["name"] = m_name;
    result["group"] = m_group;
    QJsonArray tags_arr;
    for (const auto& tag : m_tags) {
        tags_arr.append(QJsonValue(tag));
    }
    result["tags"] = tags_arr;
    result["parameters"] = m_parameters->serialize();
    result["modMatrix"] = m_modMatrix->serialize();
    return result;
}

Preset* Preset::copy() {
    auto p = new Preset(parent());
    p->parse(serialize());
    p->m_last_modified = QDateTime::currentDateTime();
    return p;
}

QString Preset::getCloneName(int n) const {
    QString name;
    QString nstr = QString::asprintf("%d", n);
    QRegExp rx(R"(^(.*) ([0-9]+))");
    if (rx.indexIn(m_name) != -1) {
        name = rx.cap(1);
    } else {
        name = m_name;
    }
    if (name.length() > 19 - nstr.length() - 1) {
        name = name.left(19 - nstr.length() - 1);
    }
    return name + " " + nstr;
}

Q_INVOKABLE bool Preset::setTag(const QString& tag, bool state) {
    if (m_tags.contains(tag)) {
        if (state) {
            return false;
        } else {
            qDebug() << "Preset" << m_name << "-tag:" << tag;
            m_tags.removeAll(tag);
        }
    } else {
        if (m_tags.count() < MAX_TAGS) {
            qDebug() << "Preset" << m_name << "+tag:" << tag;
            m_tags.append(tag);
        } else {
            return false;
        }
    }
    qDebug() << "Preset" << m_name << "tagList changed. New count:" << m_tags.count();
    m_taglistmodel->setStringList(m_tags);
    return true;
}
