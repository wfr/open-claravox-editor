// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "TagListModel.h"
#include "PresetListModel.h"

TagListModel::TagListModel(QObject *parent, const PresetListModel* presets)
    : QAbstractListModel{parent},
      m_presets(presets)
{
    connect(m_presets, &PresetListModel::rowsInserted, this, &TagListModel::refresh);
    connect(m_presets, &PresetListModel::tagsChanged, this, &TagListModel::refresh);
}

int TagListModel::rowCount(const QModelIndex& parent) const {
    return m_tags.count();
}

int TagListModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant TagListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QString();
    return m_tags[index.row()];
}

QHash<int, QByteArray> TagListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TagRole] = "tag";
    return roles;
}

void TagListModel::refresh() {
    beginResetModel();
    QSet<QString> tagset;
    const int row_count = m_presets->rowCount();
    for (int row = 0; row < row_count; row++) {
        QModelIndex index = m_presets->index(row, 0);
        QStringList tag_list = index.data(PresetListModel::TagListRole).toStringList();
        for (const QString& tag : tag_list) {
            tagset.insert(tag);
        }
    }
    m_tags.clear();
    for (const QString& tag : tagset) {
        m_tags << tag;
    }
    m_tags.sort();
    endResetModel();
}
