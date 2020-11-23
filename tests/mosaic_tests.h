#ifndef MOSAIC_TESTS_H
#define MOSAIC_TESTS_H

#include <QtTest>
#include <QPushButton>
#include "../src/Mosaic/mosaic.h"
#include <QApplication>

class MosaicTests : public QObject
{
    Q_OBJECT

public:
    MosaicTests();
    ~MosaicTests();

private slots:
    void checkGameStart();
    void onColorBtClickedTest_data();
    void onColorBtClickedTest();
    void checkGameOnPause_data();
    void checkGameOnPause();
    void checkClearField_data();
    void checkClearField();

private:
    QPushButton *changeColorBt,
                *pauseGameBt,
                *startGameBt,
                *restartGameBt;
    Mosaic *game;
};

#endif // MOSAIC_TESTS_H
