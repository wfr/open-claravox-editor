// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "BackEnd.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <iostream>
#include <QSettings>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent),
    m_current_preset(nullptr),
    m_current_preset_modified(false)
{
    m_connected = false;
    m_params = new Parameters(this);
    m_presets = new PresetListModel(this);
    m_tags = new TagListModel(this, m_presets);
    m_groups = new GroupListModel(this, m_presets);
    m_filtered_presets = new FilteredPresetListModel();
    m_filtered_presets->setSourceModel(m_presets);
    m_presets->setFavorites(settings().value("favorites", QStringList()).toStringList(), false);
    m_ui_accent_color = settings().value("uiAccentColor", "#E91E63").toString(); // default Material.Pink
    m_ui_warn_unsaved_changes = settings().value("uiWarnUnsavedChanges", true).toBool();
    m_midi_channel_in = settings().value("midiChannelIn", 0).toInt();
    m_midi_channel_out = settings().value("midiChannelOut", 1).toInt();
    m_midi_channel_app = settings().value("midiChannelApp", 1).toInt();

    connect(m_presets, &PresetListModel::rowsInserted, [] {
//       qDebug() << "PresetListModel::rowsInserted";
    });

    connect(this, &BackEnd::currentPresetChanged, [this]() {
        qDebug() << "currentPreset()" << currentPreset();
    });

    connect(m_params, &Parameters::propertyChanged, [this](QString property) {
        if (!m_current_preset_modified) {
            qDebug() << "current preset modified";
            m_current_preset_modified = true;
            emit currentPresetModified();
        }
    });

    connect(m_presets, &PresetListModel::favoritesChanged, [this]() {
        QStringList favorites = m_presets->favorites();
        settings().setValue("favorites", favorites);
    });

    connect(this, &BackEnd::uiAccentColorChanged, [this]() {
        settings().setValue("uiAccentColor", m_ui_accent_color);
    });

    connect(this, &BackEnd::midiChannelInChanged, [this]() {
        settings().setValue("midiChannelIn", m_midi_channel_in);
    });

    connect(this, &BackEnd::midiChannelOutChanged, [this]() {
        settings().setValue("midiChannelOut", m_midi_channel_out);
    });

    connect(this, &BackEnd::midiChannelAppChanged, [this]() {
        settings().setValue("midiChannelApp", m_midi_channel_app);
    });

    connect(this, &BackEnd::midiOutputResolutionChanged, [this]() {
        settings().setValue("midiOutputResolution", m_midi_output_resolution);
    });

    copyFactoryPresets();
    loadPresets();
    selectPreset(m_presets->byIndex(0)->name());
}


QString BackEnd::presetsPath() {
    QString config_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QString presets_path = QDir(config_path).filePath("Presets");
    if (!QDir(presets_path).exists()) {
        if (!QDir().mkpath(presets_path)) {
            throw new std::runtime_error("cannot create presets_dir");
        }
    }
    return presets_path;
}

void BackEnd::copyFactoryPresets() {
    QDir presets_dir = QDir(presetsPath());
    qDebug() << "Copying factory presets to:" << presets_dir.path();
    auto factory_presets_dir = QDir(":/backend/factory-presets");
    for (auto fileinfo : factory_presets_dir.entryInfoList()) {
        auto inf = QFile(fileinfo.absoluteFilePath());
        if (!inf.open(QIODevice::ReadOnly)) {
            throw new std::runtime_error("failed to open internal copy of factory preset");
        }
        auto bytes = inf.readAll();
        inf.close();

        QString dest_path = presets_dir.filePath(fileinfo.fileName());
        auto outf = QFile(dest_path);
        if (!outf.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            throw new std::runtime_error("failed to open factory preset file");
        }
        if (outf.write(bytes) < fileinfo.size()) {
            throw new std::runtime_error("failed to write factory preset file");
        }
        outf.close();
    }
}

void BackEnd::loadPresets() {
    QDir presets_dir = QDir(presetsPath());
    qDebug() << "Reading presets from:" << presets_dir.path();
    presets_dir.setNameFilters({"*.mpr"});
    for (auto fileinfo : presets_dir.entryInfoList()) {
        auto f = QFile(fileinfo.absoluteFilePath());
        if (!f.open(QIODevice::ReadOnly)) {
            throw new std::runtime_error(std::string("failed to read preset: ") + fileinfo.fileName().toStdString());
        }
        auto bytes = f.readAll();
        f.close();

        auto doc = QJsonDocument::fromJson(bytes);
        auto preset = new Preset(this, doc.object());

        preset->setProperty("lastModified", fileinfo.lastModified());
//        qDebug().noquote() << QString("Preset | Group: %1, Name: %2").arg(preset->m_group).arg(preset->m_name);
        m_presets->append(preset);
    }
}


void BackEnd::selectPreset(const QString& name) {
    Preset* preset = m_presets->byName(name);
    if (preset == nullptr) {
        qWarning() << "BackEnd::selectPreset called with invalid name";
        return;
    }
    m_params->assign(preset->params());
    if (m_current_preset != preset) {
        m_current_preset = preset;
        m_current_preset_index = m_presets->findRow(preset);
        m_current_preset_modified = false;
        emit currentPresetChanged();
        emit currentPresetIndexChanged();
        emit currentPresetModified();
    }
}

void BackEnd::renameCurrentPreset(const QString& new_name) {
    if (currentPreset()->name() == new_name) {
        qDebug() << "renameCurrentPreset failed: old and new name are identical";
        return;
    }
    if (m_presets->byName(new_name) != nullptr) {
        qDebug() << "renameCurrentPreset failed: new name already exists";
        return;
    }
    qDebug() << "Renaming current preset to" << new_name;
//    Preset* p = m_presets->byName(currentPreset()->name());
//    if (p == nullptr) {
//        qDebug() << "NOTIMPL: This is a new preset. What to do?";
//        return;
//    }
    int row = m_presets->findRow(m_current_preset);
    m_presets->setData(m_presets->index(row, 0), new_name, PresetListModel::NameRole);
    m_current_preset->setName(new_name);
}


void BackEnd::saveCurrentPreset() {
    if (!m_current_preset_modified) {
        return;
    }
    if (currentPreset()->isFactory()) {
        // auto-generate a name
        // TOOD: check existing names
        QString new_name = currentPreset()->name() + "1";
        m_presets->insertPreset(m_current_preset_index, currentPreset(), new_name);
    }
}


QSettings BackEnd::settings() {
    return QSettings("wfr.github.com", "Open Claravox Editor");
}

Q_INVOKABLE QString BackEnd::appChangelog() {
    QString result;
    QFile f(":/CHANGELOG.md");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "ERROR: could not read CHANGELOG.md";
    }
    result = f.readAll();
    f.close();
    return result;
}

Q_INVOKABLE QString BackEnd::presetsFolderURL() {
    return QUrl::fromLocalFile(presetsPath()).toString();
}