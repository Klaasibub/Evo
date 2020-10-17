#ifndef UTILS_TESTS_H
#define UTILS_TESTS_H
#include <QtTest>

class UtilsTests : public QObject
{
    Q_OBJECT

public:
    UtilsTests(){}
    ~UtilsTests(){}

private slots:
    void writeReadFromFile_data();
    void writeReadFromFile();
    void base64Converting_data();
    void base64Converting();
    void jsonValidation_data();
    void jsonValidation();
};


#endif // UTILS_TESTS_H
