// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "SortFilterPresetListModel.h"
#include <PresetListModel.h>

SortFilterPresetListModel::SortFilterPresetListModel()
{
    setSortRole(PresetListModel::ModelRoles::NameRole);
    setDynamicSortFilter(true);
    sort(0, Qt::AscendingOrder);
}

bool SortFilterPresetListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    PresetListModel* source = dynamic_cast<PresetListModel*>(sourceModel());
    Preset* preset = source->byIndex(sourceRow);

    if (!m_filter_favorites && m_tags.count() == 0 && m_groups.count() == 0) {
        // no filters? return everything.
        return true;
    }

    if (m_filter_favorites && source->isFavorite(preset->name())) {
        return true;
    }

    if (m_groups.count() && m_groups.contains(preset->group())) {
        return true;
    }

    if (m_tags.count() > 0) {
        for (const QString& tag : preset->tags()) {
            if (m_tags.contains(tag)) {
                return true;
            }
        }
    }

    return false;
}


void SortFilterPresetListModel::setFilterFavorites(bool state) {
    if (m_filter_favorites != state) {
        m_filter_favorites = state;
        invalidateFilter();
    }
}

void SortFilterPresetListModel::setFilterGroups(const QStringList& groups) {
    QSet<QString> set;
    for (const QString& group : groups) {
        set.insert(group);
    }
    if (m_groups != set) {
        m_groups = set;
        invalidateFilter();
    }
}

void SortFilterPresetListModel::setFilterGroup(const QString& group, bool state) {
    if (state && !m_groups.contains(group)) {
        m_groups.insert(group);
        invalidateFilter();
    } else if (!state && m_groups.contains(group)) {
        m_groups.remove(group);
        invalidateFilter();
    }
}

void SortFilterPresetListModel::setFilterTags(const QStringList& tags) {
    QSet<QString> set;
    for (const QString& tag : tags) {
        set.insert(tag);
    }
    if (m_tags != set) {
        m_tags = set;
        invalidateFilter();
    }
}

void SortFilterPresetListModel::setFilterTag(const QString& tag, bool state) {
    if (state && !m_tags.contains(tag)) {
        m_tags.insert(tag);
        invalidateFilter();
    } else if (!state && m_tags.contains(tag)) {
        m_tags.remove(tag);
        invalidateFilter();
    }
}

bool SortFilterPresetListModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {
    QVariant leftName = source_left.data(PresetListModel::NameRole);
    QVariant rightName = source_right.data(PresetListModel::NameRole);
    return leftName.toString() < rightName.toString();
}
