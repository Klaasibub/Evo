#include <QtTest>
#include <../src/utils.h>
#include <../src/json_schemas.h>

class UnitTests : public QObject
{
    Q_OBJECT

public:
    UnitTests(){}
    ~UnitTests(){}

private slots:
    void write_read_from_file();
    void base64_converting();
    void json_validation();
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

void UnitTests::json_validation()
{
    QJsonObject quiz
    {
        {
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
        }
    };

    QCOMPARE(utils::json_validation(quiz, json_schema::quiz_questions), true);
}


QTEST_APPLESS_MAIN(UnitTests)

#include "tst_unittests.moc"
