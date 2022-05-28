#include <QtTest>
#include <iostream>
#include "Preset.h"

class TestPresetParser : public QObject
{
    Q_OBJECT

public:
    TestPresetParser();
    ~TestPresetParser();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_parse_serialize_data();
    void test_parse_serialize();

protected:
    QString diff(const QByteArray& a, const QByteArray& b);
};

TestPresetParser::TestPresetParser()
{
}

TestPresetParser::~TestPresetParser()
{
}

void TestPresetParser::initTestCase()
{
}

void TestPresetParser::cleanupTestCase()
{
}

void TestPresetParser::test_parse_serialize_data() {
    QTest::addColumn<QFileInfo>("fileinfo");
    auto factory_presets_dir = QDir(":/backend/factory-presets");
    for (auto fileinfo : factory_presets_dir.entryInfoList()) {
        QTest::newRow(fileinfo.baseName().toStdString().c_str()) << fileinfo;
    }
}

/** For each factory preset:
 *  - normalize json --> store as A
 *  - parse A into Preset P
 *  - serialize Preset P --> store as B
 *  - test A == B
 */
void TestPresetParser::test_parse_serialize()
{
    QFETCH(QFileInfo, fileinfo);

    QByteArray in_bytes_normalized;
    QByteArray out_bytes;

    /*** Normalize input A ***/
    auto in_file = QFile(fileinfo.absoluteFilePath());
    QVERIFY(in_file.open(QIODevice::ReadOnly));
    auto in_doc = QJsonDocument::fromJson(in_file.readAll());
    in_file.close();

    // cull/ignore inactive modMatrix entries
    QJsonArray destinations = in_doc["modMatrix"]["destinations"].toArray();
    for (size_t pos = 0; pos < destinations.size(); pos++) {
        if (destinations.at(pos).isObject()) {
            QJsonObject dest = destinations.at(pos).toObject();
            if (    dest["externalCvEnable"].toBool() == false &&
                    dest["volumeCvEnable"].toBool() == false &&
                    dest["pitchCvEnable"].toBool() == false) {
                destinations.replace(pos, QJsonValue());
            }
        }
    }
    QJsonObject modmatrix_obj;
    modmatrix_obj.insert("destinations", destinations);
    QJsonObject in_obj = in_doc.object();
    in_obj.remove("modMatrix");
    in_obj.insert("modMatrix", modmatrix_obj);
    in_doc.setObject(in_obj);

    in_bytes_normalized = in_doc.toJson(QJsonDocument::JsonFormat::Indented);
    in_file.close();

    /*** Parse A into Preset P ***/
    auto preset = new Preset(this);
    QVERIFY(preset->parse(in_doc.object()));

    /*** Serialize P into B ***/
    QJsonDocument out_doc(preset->serialize());
    delete preset;
    out_bytes = out_doc.toJson(QJsonDocument::JsonFormat::Indented);

    if (in_bytes_normalized != out_bytes) {
        std::cerr << diff(in_bytes_normalized, out_bytes).toStdString() << std::endl;
    }

    QVERIFY(in_bytes_normalized == out_bytes);
}

QString TestPresetParser::diff(const QByteArray& a, const QByteArray& b) {
    QTemporaryFile temp_a;
    temp_a.open();
    temp_a.write(a);
    temp_a.close();

    QTemporaryFile temp_b;
    temp_b.open();
    temp_b.write(b);
    temp_b.close();

    QProcess process;
    process.start("/usr/bin/diff", {"-u", temp_a.fileName(), temp_b.fileName()});
    if (process.waitForStarted()) {
        process.waitForFinished();
        if (process.exitCode() == 0) {
            return "";
        } else {
            return process.readAllStandardOutput();
        }
    } else {
        qInfo() << "Non-fatal error in QProcess:" << process.errorString();
        return "";
    }
}

QTEST_APPLESS_MAIN(TestPresetParser)

#include "tst_presetparser.moc"
