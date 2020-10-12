#include <QtTest>
#include <../src/utils.h>

class UnitTests : public QObject
{
    Q_OBJECT

public:
    UnitTests(){}
    ~UnitTests(){}

private slots:
    void write_read_from_file();
    void base64_converting();
};

void UnitTests::write_read_from_file()
{
    bool encoding[] = {false, true};
    for(int i = 0; i < 2; i++){
        QString input_data = "SomeInformation", output_data;
        utils::write_to_file("test.file", input_data, encoding[i]);
        utils::read_from_file("test.file", output_data, encoding[i]);
        QCOMPARE(input_data, output_data);
    }
}

void UnitTests::base64_converting()
{
    QString original, encoded, decoded;
    original = "SomeInformation";
    encoded = utils::base64_encode(original);
    decoded = utils::base64_decode(encoded);
    QCOMPARE(original, decoded);
}

QTEST_APPLESS_MAIN(UnitTests)

#include "tst_unittests.moc"
