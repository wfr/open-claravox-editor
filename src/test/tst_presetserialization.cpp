#include <QtTest>

// add necessary includes here

class TestPresetSerialization : public QObject
{
    Q_OBJECT

public:
    TestPresetSerialization();
    ~TestPresetSerialization();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

TestPresetSerialization::TestPresetSerialization()
{

}

TestPresetSerialization::~TestPresetSerialization()
{

}

void TestPresetSerialization::initTestCase()
{

}

void TestPresetSerialization::cleanupTestCase()
{

}

void TestPresetSerialization::test_case1()
{

}

QTEST_APPLESS_MAIN(TestPresetSerialization)

#include "tst_presetserialization.moc"
