#include <QtTest>
#include "Backend.h"
#include "Preset.h"
#include <iostream>

// add necessary includes here

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
 *  - diff A vs. B
 *  TODO: skip test when /usr/bin/diff isn't available
 *        or replace with own code
 */
void TestPresetParser::test_parse_serialize()
{
    QFETCH(QFileInfo, fileinfo);

    /*** Normalize input A ***/
    auto in_file = QFile(fileinfo.absoluteFilePath());
    QVERIFY(in_file.open(QIODevice::ReadOnly));
    auto in_bytes_raw = in_file.readAll();
    auto in_doc = QJsonDocument::fromJson(in_bytes_raw);

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

    auto in_bytes_normalized = in_doc.toJson(QJsonDocument::JsonFormat::Indented);
    in_file.close();

    /*** Parse A into Preset P ***/
    auto preset = new Preset(this);
    QVERIFY(preset->parse(in_doc.object()));

    /*** Serialize P into B ***/
    QJsonDocument out_doc(preset->serialize());
    QByteArray out_bytes = out_doc.toJson(QJsonDocument::JsonFormat::Indented);

    /*** Diff A vs. B ***/
    QTemporaryFile a;
    a.open();
    QVERIFY(a.isOpen());
    a.write(in_bytes_normalized);
    a.close();

    QTemporaryFile b;
    b.open();
    QVERIFY(b.isOpen());
    b.write(out_bytes);
    b.close();

    QProcess process;
    process.start("/usr/bin/diff", {"-u", a.fileName(), b.fileName()});
    process.waitForFinished();
    if (process.exitCode() == 0) {
        // OK
    } else {
        // Note: std::cerr is only visible with `ctest --verbose`
        QByteArray diff = process.readAllStandardOutput();
        std::cerr << diff.toStdString() << std::endl;
        QVERIFY(false);
    }
}

QTEST_APPLESS_MAIN(TestPresetParser)

#include "tst_presetparser.moc"
