#include <../src/utils.h>
#include <../src/json_schemas.h>
#include <utils_tests.h>

void UtilsTests::writeReadFromFile_data()
{
    QTest::addColumn<QString>("input_data");
    QTest::addColumn<bool>("encoding");
    QTest::addColumn<bool>("decoding");
    QTest::addColumn<bool>("result");

    QTest::addRow("Without encoding, decoding") << "SomeInfo" << false << false << true;
    QTest::addRow("With encoding, decoding") << "SomeInfo" << true << true << true;
    QTest::addRow("With encoding, Without decoding") << "SomeInfo" << true << false << false;
    QTest::addRow("Without encoding, With decoding") << "SomeInfo" << false << true << false;
}

void UtilsTests::writeReadFromFile()
{
    QFETCH(QString, input_data);
    QFETCH(bool, encoding);
    QFETCH(bool, decoding);
    QFETCH(bool, result);

    QString output_data;

    utils::write_to_file("test.file", input_data, encoding);
    utils::read_from_file("test.file", output_data, decoding);
    QCOMPARE((input_data==output_data), result);
}

void UtilsTests::base64Converting_data()
{
    QTest::addColumn<QString>("original");
    QTest::addColumn<bool>("encoding");
    QTest::addColumn<bool>("decoding");
    QTest::addColumn<bool>("result");

    QTest::addRow("encode +, decode +") << "SomeInformation" << true << true << true;
    QTest::addRow("encode +, decode -") << "SomeInformation" << true << false << false;
    QTest::addRow("encode -, decode +") << "SomeInformation" << false << true << false;
}

void UtilsTests::base64Converting()
{
    QFETCH(QString, original);
    QFETCH(bool, encoding);
    QFETCH(bool, decoding);
    QFETCH(bool, result);

    QString encoded, decoded;
    encoded = (encoding ? utils::base64_encode(original): original);
    decoded = (decoding ? utils::base64_decode(encoded): encoded);

    QCOMPARE((original==decoded), result);
}

void UtilsTests::jsonValidation_data()
{
    QTest::addColumn<QJsonObject>("json");
    QTest::addColumn<QJsonObject>("schema");
    QTest::addColumn<bool>("result");

    QJsonObject quiz{{
            "Questions", QJsonArray{
                QJsonObject{
                    {"Question", "Some Question"},
                    {"Answer", "Correct Answer"},
                    {"A", "Some A Answer"},
                    {"B", "Some B Answer"},
                    {"C", "Some C Answer"},
                    {"D", "Some D Answer"},
                },
                QJsonObject{
                    {"Question", "Some Question 2"},
                    {"Answer", "Correct Answer"},
                    {"A", "Some A Answer"},
                    {"B", "Some B Answer"},
                    {"C", "Some C Answer"},
                    {"D", "Some D Answer"},
                }
            }
    }};

    QTest::newRow("Quiz questions shema") << quiz << json_schema::quiz_questions << true;
    QTest::newRow("Invalid schema") << quiz << QJsonObject{{"Invalid", "Schema"}} << false;
    QTest::newRow("Invalid json") << QJsonObject{{"Invalid", "Json"}} << json_schema::quiz_questions << false;
}

void UtilsTests::jsonValidation()
{
    QFETCH(QJsonObject, json);
    QFETCH(QJsonObject, schema);
    QFETCH(bool, result);

    QCOMPARE((utils::json_validation(json, schema)), result);
}
