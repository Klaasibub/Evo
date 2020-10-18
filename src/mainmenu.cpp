#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include "utils.h"
#include <Quiz/quiz.h>
#include <Hangman/hangman.h>
#include <Memory/memory.h>
#include <Mosaic/mosaic.h>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    check_update();

    currentGame = Game::Quiz;

    ui->mainStackedWidget->setCurrentIndex((int)MainWidget::Menu);
    ui->menuStackedWidget->setCurrentIndex((int)currentGame);

    for(auto item : {ui->leftQuizBt, ui->leftHangmanBt, ui->leftMosaicBt, ui->leftMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(previousGame()));

    for(auto item : {ui->rightQuizBt, ui->rightHangmanBt, ui->rightMosaicBt, ui->rightMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(nextGame()));

    for(auto item : {ui->playQuizBt, ui->playHangmanBt, ui->playMosaicBt, ui->playMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(play()));

    for(auto item : {ui->aboutQuizBt, ui->aboutHangmanBt, ui->aboutMosaicBt, ui->aboutMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(about()));

    for(auto item : {ui->tableQuizBt, ui->tableHangmanBt, ui->tableMosaicBt, ui->tableMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(table()));

    for(auto item : {ui->backAboutQuizBt, ui->backAboutHangmanBt, ui->backAboutMosaicBt, ui->backAboutMemoryBt,
                     ui->backTableQuizBt, ui->backTableHangmanBt, ui->backTableMosaicBt, ui->backTableMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(backToMenu()));

    for(auto item : {ui->exitQuizBt, ui->exitHangmanBt, ui->exitMosaicBt, ui->exitMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(exitGame()));

}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::previousGame()
{
    currentGame =  (Game)((numberGames + (int)(currentGame) - 1) % numberGames);
    ui->menuStackedWidget->setCurrentIndex((int)currentGame);
}

void MainMenu::nextGame()
{
    currentGame = (Game)(((int)(currentGame) + 1) % numberGames);
    ui->menuStackedWidget->setCurrentIndex((int)currentGame);
}

void MainMenu::exitGame()
{
    close();
}

void MainMenu::startGame(QDialog *game)
{
    game->setModal(true);
    setVisible(false);
    game->exec();
    setVisible(true);
}

void MainMenu::play()
{
    QDialog *game;
    switch(currentGame){
    case Game::Quiz:
        game = new Quiz;
        break;
    case Game::Hangman:
        game = new Hangman;
        break;
    case Game::Memory:
        game = new Memory;
        break;
    case Game::Mosaic:
        game = new Mosaic;
        break;
    }
    startGame(game);
    delete game;
}

void MainMenu::about()
{
    ui->aboutStackedWidget->setCurrentIndex((int)currentGame);
    ui->mainStackedWidget->setCurrentIndex((int)MainWidget::About);
}

void MainMenu::backToMenu()
{
    ui->menuStackedWidget->setCurrentIndex((int)currentGame);
    ui->mainStackedWidget->setCurrentIndex((int)MainWidget::Menu);
}

void MainMenu::table()
{
    QString recordsPath = ":/Memory/table.test"; // ToDo: replace to Game.recordPath();
    switch(currentGame)
    {
    case Game::Quiz:
        //recordsPath = Quiz::recordsPath;
        fillTable(ui->recordsQuizTable, recordsPath);
        break;
    case Game::Memory:
        //recordsPath = Memory::recordsPath;
        fillTable(ui->recordsMemoryTable, recordsPath);
        break;
    case Game::Hangman:
        //recordsPath = Hangman::recordsPath;
        fillTable(ui->recordsHangmanTable, recordsPath);
        break;
    case Game::Mosaic:
        //recordsPath = Mosaic::recordsPath;
        fillTable(ui->recordsMosaicTable, recordsPath);
        break;
    }
    ui->tableStackedWidget->setCurrentIndex((int)currentGame);
    ui->mainStackedWidget->setCurrentIndex((int)MainWidget::Table);
}

void MainMenu::fillTable(QTableWidget *table, QString filename)
{
    QString data;
    utils::read_from_file(filename, data, false);
    QStringList rowData, rowsData = data.split("\n");

    table->horizontalHeader()->setVisible(true);

    if(rowsData.size() > 0){
        rowData = rowsData.at(0).split(";");
        table->setColumnCount(rowData.size());
        for (int i = 0; i < rowData.size(); i++){
            table->setHorizontalHeaderItem(i, new QTableWidgetItem(rowData[i]));
            table->setHorizontalHeaderLabels(rowData);
            table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
    }

    table->setRowCount(rowsData.size()-1);

    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        for (int j = 0; j < rowData.size(); j++)
            table->setItem(i-1, j, new QTableWidgetItem(rowData[j]));
    }

    table->sortByColumn(2, Qt::AscendingOrder);
    table->sortByColumn(1, Qt::DescendingOrder);
    table->setSortingEnabled(false);
}

void MainMenu::check_update()
{
    auto manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        if (reply->error()) {
            ui->statusbar->showMessage(QString("Error %1").arg(reply->errorString()));
            return;
        }
        QByteArray responseData = reply->readAll();
        QJsonObject json = utils::json_loads(responseData);
        ui->statusbar->showMessage(json["info"].toString());

        reply->deleteLater();
        manager->deleteLater();
    });
    QUrl check_update_url{service_url.toString()+"/check_update/?currentVersion="+currentVerison};
    manager->get(QNetworkRequest(check_update_url));
}
