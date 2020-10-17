#ifndef MAIN_GUI_TESTS_H
#define MAIN_GUI_TESTS_H

#include <QtTest>


class MainGuiTests : public QObject
{
    Q_OBJECT

public:
    MainGuiTests(){}
    ~MainGuiTests(){}

private slots:
    void testCase1();
};

#endif // MAIN_GUI_TESTS_H
