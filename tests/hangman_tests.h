#ifndef HANGMAN_TESTS_H
#define HANGMAN_TESTS_H

#include <QtTest>

class HangmanTests : public QObject
{
    Q_OBJECT

public:
    HangmanTests(){}
    ~HangmanTests(){}

private slots:
    void testCase1();
};

#endif // HANGMAN_TESTS_H
