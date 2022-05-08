// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QSortFilterProxyModel>
#include <QObject>
#include <QSet>

/*!
 * \brief The FilteredPresetListModel provides a filtered view of a PresetListModel.
 *        It aims to behave exactly like Moog's original editor.
 *        The multiple filter options are chained with an OR relationship,
 *        e.g. [Favorites] + [Moog] shows both all favorite _and_ all Moog presets.
 */
class FilteredPresetListModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilteredPresetListModel();

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    //! show only presets with one of these tags
    void setFilterTags(const QStringList& tags);

    //! show only favorites
    void setFilterFavorites(bool);

protected:
    QSet<QString> m_tags;
    bool m_filter_favorites = false;
};
