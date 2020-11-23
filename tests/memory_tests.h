#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

#include <QtTest>

class MemoryTests : public QObject

{
    Q_OBJECT

public:
    MemoryTests(){}
    ~MemoryTests(){}

private slots:
    void testCase1();
};

#endif // MEMORY_TESTS_H
