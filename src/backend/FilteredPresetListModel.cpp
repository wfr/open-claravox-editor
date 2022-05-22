// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "FilteredPresetListModel.h"
#include <PresetListModel.h>

FilteredPresetListModel::FilteredPresetListModel()
{

}

bool FilteredPresetListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
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


void FilteredPresetListModel::setFilterFavorites(bool state) {
    if (m_filter_favorites != state) {
        m_filter_favorites = state;
        invalidateFilter();
    }
}

void FilteredPresetListModel::setFilterGroups(const QStringList& groups) {
    QSet<QString> set;
    for (const QString& group : groups) {
        set.insert(group);
    }
    if (m_groups != set) {
        m_groups = set;
        invalidateFilter();
    }
}

void FilteredPresetListModel::setFilterGroup(const QString& group, bool state) {
    if (state && !m_groups.contains(group)) {
        m_groups.insert(group);
        invalidateFilter();
    } else if (!state && m_groups.contains(group)) {
        m_groups.remove(group);
        invalidateFilter();
    }
}

void FilteredPresetListModel::setFilterTags(const QStringList& tags) {
    QSet<QString> set;
    for (const QString& tag : tags) {
        set.insert(tag);
    }
    if (m_tags != set) {
        m_tags = set;
        invalidateFilter();
    }
}

void FilteredPresetListModel::setFilterTag(const QString& tag, bool state) {
    if (state && !m_tags.contains(tag)) {
        m_tags.insert(tag);
        invalidateFilter();
    } else if (!state && m_tags.contains(tag)) {
        m_tags.remove(tag);
        invalidateFilter();
    }
}
