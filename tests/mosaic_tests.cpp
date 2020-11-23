#include "mosaic_tests.h"

MosaicTests::MosaicTests()
{
    game = new Mosaic;
    changeColorBt = new QPushButton;
    pauseGameBt = new QPushButton;
    startGameBt = new QPushButton;
    restartGameBt = new QPushButton;

    game->readImages();

    connect(changeColorBt, SIGNAL(clicked()),game, SLOT(onColorBtClicked()));
    connect(pauseGameBt, SIGNAL(clicked()),game, SLOT(on_pauseBt_clicked()));
    connect(startGameBt, SIGNAL(clicked()),game, SLOT(on_startBt_clicked()));
    connect(restartGameBt, SIGNAL(clicked()),game, SLOT(on_restartBt_clicked()));
}

MosaicTests::~MosaicTests()
{
    delete game;
    delete changeColorBt;
    delete pauseGameBt;
    delete startGameBt;
    delete restartGameBt;
}

void MosaicTests::checkGameStart()
{
    startGameBt->click();
    bool game_not_on_pause = game->gameNotOnPause();
    QVERIFY(game_not_on_pause);
}

void MosaicTests::onColorBtClickedTest_data()
{
    QTest::addColumn<QColor>("buttonColor");
    QTest::addColumn<QColor>("expectedColor");
    QTest::addColumn<bool>("result");

    QTest::addRow("buttonColor == expectedColor") << QColor(255, 135, 0) << QColor(255, 135, 0) << true;
    QTest::addRow("buttonColor != expectedColor") << QColor(0, 135, 255) << QColor(255, 135, 0) << false;
}
void MosaicTests::onColorBtClickedTest()
{
    QFETCH(QColor, buttonColor);
    QFETCH(QColor, expectedColor);
    QFETCH(bool, result);

    changeColorBt->setAutoFillBackground(true);
    changeColorBt->setPalette(QPalette(buttonColor));

    changeColorBt->click();
    QColor currentColor = game->getCurrentColor();

    QCOMPARE((currentColor == expectedColor), result);
}


void MosaicTests::checkGameOnPause_data()
{
    QTest::addColumn<bool>("use_pause");
    QTest::addColumn<bool>("use_unpause");
    QTest::addColumn<bool>("result");

    QTest::addRow("paused, resumed") << true << true << true;
    QTest::addRow("only resumed") << false << true << true;
    QTest::addRow("only paused") << true << false << false;
    QTest::addRow("nothing") << false << false << true;
}

void MosaicTests::checkGameOnPause()
{
    QFETCH(bool, use_pause);
    QFETCH(bool, use_unpause);
    QFETCH(bool, result);

    startGameBt->click();
    if(use_pause)
        pauseGameBt->click();
    if(use_unpause)
        restartGameBt->click();

    bool game_not_on_pause = game->gameNotOnPause();
    QCOMPARE(game_not_on_pause, result);
}

void MosaicTests::checkClearField_data()
{
    QTest::addColumn<int>("fieldSize_row");
    QTest::addColumn<int>("fieldSize_column");

    QTest::addRow("tableSize [5x5]") << 5 << 5;
    QTest::addRow("tableSize [10x8]") << 10 << 8;
    QTest::addRow("tableSize [1x1]") << 1 << 1;
}

void MosaicTests::checkClearField()
{
    QFETCH(int, fieldSize_row);
    QFETCH(int, fieldSize_column);
    QTableWidget *field1 = new QTableWidget;
    QTableWidget *field2 = new QTableWidget;

    //заполнение матрицы цветами
    field2->setColumnCount(fieldSize_column);
    field2->setRowCount(fieldSize_row);
    field1->setColumnCount(fieldSize_column);
    field1->setRowCount(fieldSize_row);
    for (int i = 0; i < fieldSize_row; i++ ) {
        for (int j = 0; j < fieldSize_column; j++ ) {
            field2->setItem(i,j,new QTableWidgetItem());
            field1->setItem(i,j,new QTableWidgetItem());
            QColor color(rand()%255,rand()%255,rand()%255);
            field1->item(i, j)->setBackground(QBrush(color));
            field2->item(i, j)->setBackground(QBrush(QColor(255,255,255)));
        }
    }
    QVERIFY(!game->compareTables(field1,field2));
    game->clearField(field1);
    QVERIFY(game->compareTables(field1,field2));
    delete field1;
    delete field2;
}
