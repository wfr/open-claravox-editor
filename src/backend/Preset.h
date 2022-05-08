// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QStringListModel>
#include "Parameters.h"

class Preset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString instrument MEMBER m_instrument)
    Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
    Q_PROPERTY(QString group MEMBER m_group NOTIFY groupChanged)
    Q_PROPERTY(QStringListModel* tagList READ tagListModel NOTIFY tagListChanged)
    Q_PROPERTY(Parameters* parameters MEMBER m_parameters)
    Q_PROPERTY(QDateTime lastModified MEMBER m_last_modified NOTIFY lastModifiedChanged)
    Q_PROPERTY(bool isFactory READ isFactory NOTIFY isFactoryChanged)
public:
    explicit Preset(QObject *parent = nullptr);
    Preset(QObject *parent, const QJsonObject& obj);

    QString name() const {
        return m_name;
    }

    void setName(const QString& new_name) {
        // TODO: validate
        m_name = new_name;
        emit nameChanged();
    }

    QString group() const {
        return m_group;
    }

    QString tagString() const {
        return m_tags.join(" ");
    }

    QStringList tags() const {
        return QStringList(m_tags);
    }

    QStringListModel* tagListModel() const {
        return m_taglistmodel;
    }

    const Parameters& params() const {
        return *m_parameters;
    }

    bool isFactory() const {
        return (m_group == "Moog" ||
                m_group == "Dorit Chrysler" ||
                m_group == "Grégoire Blanc");
    }

    QJsonObject serialize();
    Preset* clone();

signals:
    void nameChanged();
    void groupChanged();
    void tagsChanged();
    void tagListChanged();
    void lastModifiedChanged();
    void isFactoryChanged();

public:
    QString m_instrument;
    QString m_name;
    QString m_group;
    QList<QString> m_tags;
    Parameters* m_parameters;
    QDateTime m_last_modified;

protected:
    QStringListModel* m_taglistmodel;
};

Q_DECLARE_METATYPE(Preset*)
Q_DECLARE_METATYPE(const Preset*)
