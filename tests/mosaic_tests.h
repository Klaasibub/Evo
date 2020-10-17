#ifndef MOSAIC_TESTS_H
#define MOSAIC_TESTS_H

#include <QtTest>

class MosaicTests : public QObject
{
    Q_OBJECT

public:
    MosaicTests(){}
    ~MosaicTests(){}

private slots:
    void testCase1();
};

#endif // MOSAIC_TESTS_H
