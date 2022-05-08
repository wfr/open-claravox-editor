// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QSet>

class PresetListModel;

class TagListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TagListModel(QObject *parent, const PresetListModel* m_presets);

    enum ModelRoles {
        TagRole = Qt::UserRole + 1,
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    const PresetListModel* m_presets;
    QStringList m_tags;
};
