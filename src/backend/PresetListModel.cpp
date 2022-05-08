// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "PresetListModel.h"

PresetListModel::PresetListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PresetListModel::rowCount(const QModelIndex& parent) const
{
    return m_presets.size();
}

int PresetListModel::columnCount(const QModelIndex& parent) const
{
    return 2; // ???
}

QVariant PresetListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    const Preset* preset = m_presets[index.row()];
    switch (role) {
    case NameRole:
        return preset->name();
    case GroupRole:
        return preset->group();
    case TagStringRole:
        return preset->tagString();
    case TagListRole:
        return preset->tags();
    case FavoriteRole:
        return m_favorites.contains(preset->name());
    }
    return QVariant();
}

QHash<int, QByteArray> PresetListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TagStringRole] = "tagstring";
    roles[GroupRole] = "group";
    roles[TagListRole] = "tags";
    roles[FavoriteRole] = "favorite";
    return roles;
}

bool PresetListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    Preset* preset = m_presets[index.row()];
    switch (role) {
    case NameRole:
        preset->setName(value.toString());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags PresetListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void PresetListModel::append(Preset* preset) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_presets.append(preset);
    endInsertRows();
}

Preset* PresetListModel::byName(const QString& name) {
    for (const auto preset : m_presets) {
        if (preset->name() == name) {
            return preset;
        }
    }
    return nullptr;
}

Preset* PresetListModel::byIndex(int index) {
    return m_presets.at(index);
}

Q_INVOKABLE int PresetListModel::findRow(const Preset* needle) {
    int row = 0;
    for (const auto preset : m_presets) {
        if (preset == needle) {
            return row;
        }
        row++;
    }
    return -1;
}

void PresetListModel::insertPreset(int row, const Preset* preset, const QString& new_name) {
    beginInsertRows(QModelIndex(), row, row);
    Preset* new_preset = m_presets[row]->clone();
    new_preset->setName(new_name);
    m_presets.insert(row, new_preset);
    endInsertRows();
}

void PresetListModel::setFavorite(const QString& name, bool state) {
    // Q: should we use setData() instead?
    Preset* preset = byName(name);
    if (preset != nullptr && state != isFavorite(name)) {
        if (state) {
            m_favorites.insert(name);
        } else {
            m_favorites.remove(name);
        }
        emit favoritesChanged();
        int row = findRow(preset);
        emit dataChanged(index(row), index(row), {FavoriteRole});
    }
}

void PresetListModel::toggleFavorite(const QString& name) {
    setFavorite(name, !isFavorite(name));
}

bool PresetListModel::isFavorite(const QString& name) {
    return m_favorites.contains(name);
}

QStringList PresetListModel::favorites() {
    QStringList favorites;
    for (auto& name : m_favorites) {
        if (byName(name) != nullptr) {
            favorites.append(name);
        }
    }
    return favorites;
}

void PresetListModel::setFavorites(const QStringList& favorites, bool notify) {
    for (const auto& name : favorites) {
        m_favorites.insert(name);
    }
    if (notify) {
        emit favoritesChanged();
    }
    // This function is only called once during initialization.
    // If this ever changes, implement proper model handling/events here.
}
