#ifndef QUIZ_TESTS_H
#define QUIZ_TESTS_H

#include <QtTest>

class QuizTests : public QObject
{
    Q_OBJECT

public:
    QuizTests(){}
    ~QuizTests(){}

private slots:
    void testCase1();
};

#endif // QUIZ_TESTS_H
