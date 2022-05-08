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

    if (m_tags.count() == 0 && !m_filter_favorites) {
        // no filters? return everything.
        return true;
    }

    if (m_filter_favorites && source->isFavorite(preset->name())) {
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

void FilteredPresetListModel::setFilterTags(const QStringList& tags) {
    m_tags.clear();
    for (const QString& tag : tags) {
        m_tags.insert(tag);
    }
}

void FilteredPresetListModel::setFilterFavorites(bool state) {
    m_filter_favorites = state;
}
