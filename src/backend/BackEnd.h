// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#pragma once
// https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <QSettings>
#include "Preset.h"
#include "PresetListModel.h"
#include "TagListModel.h"
#include "GroupListModel.h"
#include "FilteredPresetListModel.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected MEMBER m_connected NOTIFY connectedChanged)
    Q_PROPERTY(Parameters* params MEMBER m_params NOTIFY paramsChanged)
    Q_PROPERTY(PresetListModel* presets MEMBER m_presets NOTIFY presetsChanged)
    Q_PROPERTY(TagListModel* tags MEMBER m_tags NOTIFY tagsChanged)
    Q_PROPERTY(GroupListModel* groups MEMBER m_groups NOTIFY groupsChanged)
    Q_PROPERTY(Preset* currentPreset READ currentPreset NOTIFY currentPresetChanged)
    Q_PROPERTY(int currentPresetIndex READ currentPresetIndex NOTIFY currentPresetIndexChanged)
    Q_PROPERTY(bool currentPresetModified MEMBER m_current_preset_modified NOTIFY currentPresetModified)
    Q_PROPERTY(FilteredPresetListModel* filteredPresets MEMBER m_filtered_presets NOTIFY filteredPresetsChanged)
    // maybe move to a dedicated Preferences model?
    Q_PROPERTY(QString uiAccentColor MEMBER m_ui_accent_color NOTIFY uiAccentColorChanged)
    Q_PROPERTY(bool uiWarnUnsavedChanges MEMBER m_ui_warn_unsaved_changes NOTIFY uiWarnUnsavedChangesChanged)
    Q_PROPERTY(int midiChannelIn MEMBER m_midi_channel_in NOTIFY midiChannelInChanged)
    Q_PROPERTY(int midiChannelOut MEMBER m_midi_channel_out NOTIFY midiChannelOutChanged)
    Q_PROPERTY(int midiChannelApp MEMBER m_midi_channel_app NOTIFY midiChannelAppChanged)
    Q_PROPERTY(int midiOutputResolution MEMBER m_midi_output_resolution NOTIFY midiOutputResolutionChanged)
    QML_ELEMENT

public:
    explicit BackEnd(QObject *parent = nullptr);

    Parameters* params() {
        return m_params;
    }

    PresetListModel* presets() {
        return m_presets;
    }

    TagListModel* tags() { // maybe rename to tagList
        return m_tags; // and m_tag_list respectively
    }

    GroupListModel* groups() {
        return m_groups;
    }

    Preset* currentPreset() {
        return m_current_preset;
    }

    int currentPresetIndex() {
        return m_current_preset_index;
    }

    Q_INVOKABLE void selectPreset(const QString& name);
    Q_INVOKABLE void renameCurrentPreset(const QString& new_name);
    Q_INVOKABLE void saveCurrentPreset();

    Q_INVOKABLE QString appChangelog();
    Q_INVOKABLE QString presetsFolderURL();

    QSettings settings();

signals:
    void presetsChanged();
    void connectedChanged();
    void paramsChanged();
    void tagsChanged();
    void groupsChanged();
    void currentPresetChanged();
    void currentPresetIndexChanged();
    void currentPresetModified();
    void filteredPresetsChanged();
    void uiAccentColorChanged();
    void uiWarnUnsavedChangesChanged();
    void midiChannelInChanged();
    void midiChannelOutChanged();
    void midiChannelAppChanged();
    void midiOutputResolutionChanged();

private:
    bool m_connected;
    Parameters* m_params;
    PresetListModel* m_presets;
    TagListModel* m_tags;
    GroupListModel* m_groups;
    Preset* m_current_preset;
    int m_current_preset_index;
    bool m_current_preset_modified;
    FilteredPresetListModel* m_filtered_presets;
    QString m_ui_accent_color;
    bool m_ui_warn_unsaved_changes;
    int m_midi_channel_in;
    int m_midi_channel_out;
    int m_midi_channel_app;
    int m_midi_output_resolution;

    QString presetsPath();
    void copyFactoryPresets();
    void loadPresets();
};
