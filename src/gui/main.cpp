// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
#include <iostream>
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QTimer>
#include <QLoggingCategory>
#include <QMap>
#include <QScreen>

#include <deque>
#include <chrono>

#include "Claravox.h"
#include "Backend.h"

void installDefaultFont()
{
    QString font = "://fonts/NimbusSanL-Regu.ttf";
    qint32 fontId = QFontDatabase::addApplicationFont(font);
    QStringList fontList = QFontDatabase::applicationFontFamilies(fontId);
    QString family = fontList.first();
    QGuiApplication::setFont(QFont(family));
}


void printmsg(std::vector<uint8_t>& msg) {
    std::cout << "OUT: [" << int(msg[0]) << "," << int(msg[1]) << "," << int(msg[2]) << "]" << std::endl;
}

Claravox claravox;


void sendval(const QString& name, double val) {
    auto param = mpFromName(name);
    if (!param.has_value()) {
        throw std::runtime_error("sendval: invalid name");
    }

    auto messages = midiMessagesFromDouble(*param, val, 0);
    for (MIDIMessage& msg : messages) {
        claravox.send(msg);
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Open Claravox Editor");
    QCoreApplication::setApplicationVersion("0.1");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv); // Pure QML
#else
    QApplication app(argc, argv); // QML + Widgets (e.g. native dialogs)
#endif

    QQmlApplicationEngine engine;
    installDefaultFont();

    auto screen = QGuiApplication::primaryScreen();
    const qreal dpi = screen->logicalDotsPerInch();
    qDebug() << "Primary screen DPI = " << dpi;

    Backend* backend = new Backend();

    qmlRegisterUncreatableType<Parameters,1>("Claravox.Parameters",1,0,"Parameters","Failed to register QML Parameters");
    qmlRegisterUncreatableType<Preset,1>("Claravox.Preset",1,0,"Preset","Failed to register QML Preset");
    qmlRegisterUncreatableType<PresetListModel,1>("Claravox.PresetListModel",1,0,"PresetListModel","Failed to register QML PresetListModel");
    qmlRegisterUncreatableType<TagListModel,1>("Claravox.TagListModel",1,0,"TagListModel","Failed to register QML TagListModel");
    qmlRegisterUncreatableType<GroupListModel,1>("Claravox.GroupListModel",1,0,"GroupListModel","Failed to register QML GroupListModel");
    qmlRegisterUncreatableType<FilteredPresetListModel,1>("Claravox.FilteredPresetListModel",1,0,"FilteredPresetListModel","Failed to register QML FilteredPresetListModel");
    qmlRegisterSingletonInstance("Claravox.Backend", 1, 0, "Backend", backend);

    QObject::connect(backend->params(), &Parameters::pitchCurveValChanged, [](double val) {
            qDebug() << "pitchCurveVal" << val;
            sendval("pitchCurveVal", val);
    });
    QObject::connect(backend->params(), &Parameters::volCurveValChanged, [](double val) {
            qDebug() << "volCurveVal" << val;
            sendval("volCurveVal", val);
    });
    QObject::connect(backend->params(), &Parameters::quantizeAmountChanged, [](double val) {
            qDebug() << "quantizeAmount" << val;
            sendval("quantizeAmount", val);
    });
    QObject::connect(backend->params(), &Parameters::rootValChanged, [](double val) {
            qDebug() << "rootVal" << val;
            sendval("rootVal", val);
    });
    QObject::connect(backend->params(), &Parameters::scaleValChanged, [](double val) {
            qDebug() << "scaleVal" << val;
            sendval("scaleVal", val);
    });
    QObject::connect(backend->params(), &Parameters::octaveStateChanged, [](double val) {
            qDebug() << "octaveState" << val;
            sendval("octaveState", val);
    });
    QObject::connect(backend->params(), &Parameters::brightValChanged, [](double val) {
            qDebug() << "brightVal" << val;
            sendval("brightVal", val);
    });
    QObject::connect(backend->params(), &Parameters::filterValChanged, [](double val) {
            qDebug() << "filterVal" << val;
            sendval("filterVal", val);
    });
    QObject::connect(backend->params(), &Parameters::waveValChanged, [](double val) {
            qDebug() << "waveVal" << val;
            sendval("waveVal", val);
    });
    QObject::connect(backend->params(), &Parameters::delayAmountChanged, [](double val) {
            qDebug() << "delayAmount" << val;
            sendval("delayAmount", val);
    });
    QObject::connect(backend->params(), &Parameters::feedbackValChanged, [](double val) {
            qDebug() << "feedbackVal" << val;
            sendval("feedbackVal", val);
    });
    QObject::connect(backend->params(), &Parameters::timeValChanged, [](double val) {
            qDebug() << "timeVal" << val;
            sendval("timeVal", val);
    });
    QObject::connect(backend->params(), &Parameters::osc1WavetableChanged, [](double val) {
            qDebug() << "osc1Wavetable" << val;
            sendval("osc1Wavetable", val);
    });


    // MIDI handling (unfinished)
    QObject::connect(&claravox, &Claravox::connected, []() {
        std::cout << "Claravox connected." << std::endl;
    });
    QObject::connect(&claravox, &Claravox::disconnected, []() {
        std::cout << "Claravox disconnected." << std::endl;
    });
    QObject::connect(&claravox, &Claravox::messageReceived, [backend](double, MIDIMessage message) {
        if (message.isCC()) {
            unsigned char command = message.bytes[1];
            unsigned char data = message.bytes[2];
            unsigned char cc = command & ~32; // CC without LSB marker
            unsigned char msb, lsb = 0;
            bool is_fine = command & 32;

            if (auto mp = mpFromCC(cc); mp.has_value()) {
                double val;
                if (mp->max_value > 127) {
                    if (is_fine) {
                        msb = static_cast<unsigned char>(backend->params()->property(mp->name).toDouble() * 128.0);
                        lsb = data;
                    } else {
                        msb = data;
                        lsb = 0;
                    }
                    val = ((msb << 7) + lsb) / 16383.0;
                } else {
                    val = double(data) / mp->max_value;
                }
                backend->params()->setProperty(mp->name, val);
            }/* else {
                std::cout << "unknown cc " << int(cc) << std::endl;
            }*/
        }
    });
    claravox.start();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // DEBUG: log overwritten QML property bindings
    // https://doc.qt.io/qt-5/qtqml-syntax-propertybinding.html#debugging-overwriting-of-bindings
//    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));

    return app.exec();
}
