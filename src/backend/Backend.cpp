// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include "Backend.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <iostream>
#include <QSettings>

Backend::Backend(QObject *parent) :
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

    qDebug() << "settings path: " << settings().fileName();

    connect(m_presets, &PresetListModel::rowsInserted, [] {
//       qDebug() << "PresetListModel::rowsInserted";
    });

    connect(this, &Backend::currentPresetChanged, [this]() {
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

    connect(m_presets, &PresetListModel::dataChanged, [this]() {
        m_filtered_presets->invalidate();
        emit currentFilteredPresetIndexChanged();
    });

    connect(this, &Backend::uiAccentColorChanged, [this]() {
        settings().setValue("uiAccentColor", m_ui_accent_color);
    });

    connect(this, &Backend::midiChannelInChanged, [this]() {
        settings().setValue("midiChannelIn", m_midi_channel_in);
    });

    connect(this, &Backend::midiChannelOutChanged, [this]() {
        settings().setValue("midiChannelOut", m_midi_channel_out);
    });

    connect(this, &Backend::midiChannelAppChanged, [this]() {
        settings().setValue("midiChannelApp", m_midi_channel_app);
    });

    connect(this, &Backend::midiOutputResolutionChanged, [this]() {
        settings().setValue("midiOutputResolution", m_midi_output_resolution);
    });

    copyFactoryPresets();
    loadPresets();
    selectPreset(m_presets->byIndex(0)->name());
}


QString Backend::presetsDir() {
    QDir config_dir = QFileInfo(settings().fileName()).dir();
    QString presets_path = config_dir.filePath("Presets");
    if (!QDir(presets_path).exists()) {
        if (!QDir().mkpath(presets_path)) {
            throw new std::runtime_error("cannot create presets_path");
        }
    }
    return presets_path;
}

QString Backend::presetPath(const Preset* p) {
    return QDir(presetsDir()).absoluteFilePath(p->name() + ".mpr");
}

void Backend::copyFactoryPresets() {
    QDir presets_dir = QDir(presetsDir());
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

void Backend::loadPresets() {
    QDir presets_dir = QDir(presetsDir());
    qDebug() << "Reading presets from:" << presets_dir.path();
    presets_dir.setNameFilters({"*.mpr"});
    for (auto fileinfo : presets_dir.entryInfoList()) {
        auto f = QFile(fileinfo.absoluteFilePath());
        if (!f.open(QIODevice::ReadOnly)) {
            throw new std::runtime_error(std::string("failed to read preset: ") + fileinfo.fileName().toStdString());
        }
        auto bytes = f.readAll();
        f.close();

        qDebug() << "Parse preset:" << fileinfo.baseName();
        auto doc = QJsonDocument::fromJson(bytes);
        auto preset = new Preset(this);
        if (preset->parse(doc.object())) {
            preset->setProperty("lastModified", fileinfo.lastModified());
            m_presets->append(preset);
        } else {
            // TODO: display warning in GUI
            qWarning() << "Ignoring invalid preset: " << fileinfo.baseName();
            delete preset;
        }
    }
}

void Backend::selectPreset(const QString& name) {
    Preset* preset = m_presets->byName(name);
    if (preset == nullptr) {
        qWarning() << "Backend::selectPreset called with invalid name";
        return;
    }
    m_params->assign(preset->params());
    if (m_current_preset != preset) {
        m_current_preset = preset;
        m_current_preset_index = m_presets->findRow(preset);
        m_current_preset_modified = false;
        emit currentPresetChanged();
        emit currentPresetIndexChanged();
        emit currentFilteredPresetIndexChanged();
        emit currentPresetModified();
    }
}

void Backend::deleteCurrentPreset() {
    if (m_presets->rowCount() <= 1) {
        // TODO: let the user delete the last preset
        return;
    }
    QString preset_path = presetPath(currentPreset());
    QFile(preset_path).remove();
    qDebug() << "Deleted" << preset_path;
    m_presets->deletePreset(m_current_preset_index);

    int new_index = std::max(0, m_current_preset_index - 1);
    selectPreset(m_presets->byIndex(new_index)->name());
}

bool Backend::renameCurrentPreset(const QString& new_name) {
    if (currentPreset()->isFactory()) {
        qDebug() << "can't rename factory presets";
        return false;
    }
    if (currentPreset()->name() == new_name) {
        qDebug() << "renameCurrentPreset failed: old and new name are identical";
        return false;
    }
    if (m_presets->byName(new_name) != nullptr) {
        qDebug() << "renameCurrentPreset failed: new name already exists";
        return false;
    }
    qDebug() << "Renaming current preset to" << new_name;
    QFile(presetPath(currentPreset())).remove();
    int row = m_presets->findRow(m_current_preset);
    m_presets->setData(m_presets->index(row, 0), new_name, PresetListModel::NameRole);
    m_current_preset->setName(new_name);
    m_current_preset_modified = true;
    saveCurrentPreset();
    return true;
}

void Backend::saveCurrentPreset() {
    if (!m_current_preset_modified) {
        return;
    }
    Preset* p = currentPreset();
    if (p->isFactory()) {
        cloneCurrentPreset(false);
        p = currentPreset();
    }
    QJsonDocument doc(p->serialize());
    QString preset_path = presetPath(p);
    qDebug() << "Saving preset to:" << preset_path;
    auto f = QFile(preset_path);
    if (!f.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        throw new std::runtime_error(std::string("failed to write preset: ") + preset_path.toStdString());
    }
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
    m_current_preset_modified = false;
    emit currentPresetModified();
}

void Backend::cloneCurrentPreset(bool save) {
    Preset* p = currentPreset()->copy();
    QString name;
    int n = 0;
    while (!Preset::isValidName(name) || m_presets->byName(name) != nullptr) {
        n++;
        name = p->getCloneName(n);
    }
    p->setName(name);
    p->setGroupToUser();
    m_presets->insertPreset(m_current_preset_index + 1, p);
    selectPreset(name);
    if (save) {
        saveCurrentPreset();
    }
}


QSettings Backend::settings() {
    return QSettings("Claravox", "open-claravox-editor");
}

Q_INVOKABLE QString Backend::appChangelog() {
    QString result;
    QFile f(":/CHANGELOG.md");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "ERROR: could not read CHANGELOG.md";
    }
    result = f.readAll();
    f.close();
    return result;
}

Q_INVOKABLE QString Backend::presetsFolderURL() {
    return QUrl::fromLocalFile(presetsDir()).toString();
}
