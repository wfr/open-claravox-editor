// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "GroupListModel.h"
#include "PresetListModel.h"

GroupListModel::GroupListModel(QObject *parent, const PresetListModel* presets)
    : QAbstractListModel{parent},
      m_presets(presets)
{
    connect(m_presets, &PresetListModel::rowsInserted, [this]() {
        // TODO: Optimize
        // At the moment we're recreating the entire model whenever the preset list changes,
        // which is inefficient, but simple.
        beginResetModel();
        QSet<QString> set;
        const int row_count = m_presets->rowCount();
        for (int row = 0; row < row_count; row++) {
            QModelIndex index = m_presets->index(row, 0);
            QString group = index.data(PresetListModel::GroupRole).toString();
            set.insert(group);
        }
        m_groups.clear();
        for (const QString& group : set) {
            m_groups << group;
        }
        m_groups.sort();
        endResetModel();
    });
}

int GroupListModel::rowCount(const QModelIndex& parent) const {
    return m_groups.count();
}

int GroupListModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant GroupListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QString();
    return m_groups[index.row()];
}

QHash<int, QByteArray> GroupListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[GroupRole] = "group";
    return roles;
}


