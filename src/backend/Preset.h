// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QStringListModel>
#include "Parameters.h"
#include "ModMatrix.h"

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
    Q_PROPERTY(ModMatrix* modMatrix MEMBER m_modMatrix NOTIFY modMatrixChanged)
public:
    explicit Preset(QObject *parent = nullptr);

    bool parse(const QJsonObject& obj);
    QJsonObject serialize() const;
    Preset* copy();

    const size_t MAX_TAGS = 3;

    QString name() const {
        return m_name;
    }

    static bool isValidName(const QString& n) {
        QRegExp rx(R"(^[0-9A-Za-z ^$+]{1,19})");
        return rx.exactMatch(n);
    }

    void setName(const QString& new_name) {
        if (isValidName(new_name)) {
            m_name = new_name;
            emit nameChanged();
        } else {
            throw std::runtime_error("Preset::setName() with invalid name.");
        }
    }

    QString getCloneName(int n) const;

    QString group() const {
        return m_group;
    }

    void setGroupToUser() {
        m_group = "User";
        emit groupChanged();
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

    Q_INVOKABLE bool setTag(const QString& tag, bool state);

    Q_INVOKABLE bool hasTag(const QString& tag) {
        return m_tags.contains(tag);
    }

    bool isFactory() const {
        return (m_group == "Moog" ||
                m_group == "Dorit Chrysler" ||
                m_group == "Grégoire Blanc");
    }

    ModMatrix* modMatrix() const {
        return m_modMatrix;
    }

signals:
    void nameChanged();
    void groupChanged();
    void tagsChanged();
    void tagListChanged();
    void lastModifiedChanged();
    void isFactoryChanged();
    void modMatrixChanged();

public:
    QString m_instrument;
    QString m_name;
    QString m_group;
    QList<QString> m_tags;
    Parameters* m_parameters;
    QDateTime m_last_modified;

protected:
    QStringListModel* m_taglistmodel;
    ModMatrix* m_modMatrix;
};

Q_DECLARE_METATYPE(Preset*)
Q_DECLARE_METATYPE(const Preset*)
