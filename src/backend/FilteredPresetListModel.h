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

    //! show only favorites
    Q_INVOKABLE void setFilterFavorites(bool);

    //! show only certain groups
    Q_INVOKABLE void setFilterGroups(const QStringList& groups);
    Q_INVOKABLE void setFilterGroup(const QString& group, bool state);

    //! show only presets with one of these tags
    Q_INVOKABLE void setFilterTags(const QStringList& tags);
    Q_INVOKABLE void setFilterTag(const QString& tag, bool state);

protected:
    bool m_filter_favorites = false;
    QSet<QString> m_groups;
    QSet<QString> m_tags;
};
