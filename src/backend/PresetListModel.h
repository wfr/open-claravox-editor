// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once

#include <QAbstractListModel>
#include <QObject>
#include "Preset.h"

/**
 * @brief The PresetListModel class
 *
 * https://wiki.qt.io/How_to_use_QAbstractListModel
 * https://qml.guide/data-models-deconstructed-part-2-qabstractlistmodel/
 */

class PresetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PresetListModel(QObject* parent = nullptr);

    enum ModelRoles {
        NameRole = Qt::UserRole + 1,
        GroupRole,
        TagStringRole,
        TagListRole,
        FavoriteRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const override;

    void append(Preset* preset);

    // Retrieve a preset by name. This requires names to be unique.
    Q_INVOKABLE Preset* byName(const QString& name);
    Q_INVOKABLE Preset* byIndex(int index);
    Q_INVOKABLE int findRow(const Preset* needle);

    void insertPreset(int row, Preset* preset);
    void insertPresetClone(int row, const Preset* preset, const QString& new_name);
    void deletePreset(int row);

    Q_INVOKABLE void setFavorite(const QString& name, bool state);
    Q_INVOKABLE void toggleFavorite(const QString& name);
    bool isFavorite(const QString& name);
    QStringList favorites();
    void setFavorites(const QStringList& favorites, bool notify=true);

    Q_INVOKABLE bool setTag(const QString& name, const QString& tag, bool state);

signals:
//    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void favoritesChanged();
    void tagsChanged();

private:
    QList<Preset*> m_presets;
    QSet<QString> m_favorites;
};
