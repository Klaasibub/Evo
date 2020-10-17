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

    currentGame = 0;

    for(auto item : {ui->leftQuizBt, ui->leftHangmanBt, ui->leftMosaicBt, ui->leftMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(previousGame()));

    for(auto item : {ui->rightQuizBt, ui->rightHangmanBt, ui->rightMosaicBt, ui->rightMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(nextGame()));

    for(auto item : {ui->aboutQuizBt, ui->aboutHangmanBt, ui->aboutMosaicBt, ui->aboutMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(on_aboutBt_clicked()));

    for(auto item : {ui->backAboutQuizBt, ui->backAboutHangmanBt, ui->backAboutMosaicBt, ui->backAboutMemoryBt,
                     ui->backTableQuizBt, })
        connect(item, SIGNAL(clicked()),this,SLOT(on_backToMenuBt_clicked()));

    for(auto item : {ui->exitQuizBt, ui->exitHangmanBt, ui->exitMosaicBt, ui->exitMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(exitGame()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::previousGame()
{
    currentGame = (numberGames+currentGame-1)%numberGames;
    ui->menuStackedWidget->setCurrentIndex(currentGame);
}

void MainMenu::nextGame()
{
    currentGame = (currentGame+1)%numberGames;
    ui->menuStackedWidget->setCurrentIndex(currentGame);
}

void MainMenu::exitGame()
{
    exit(0);
}

void MainMenu::startGame(QDialog *game)
{
    game->setModal(true);
    setVisible(false);
    game->exec();
    setVisible(true);
}

void MainMenu::on_playQuizBt_clicked()
{
    Quiz *game = new Quiz;
    startGame(game);
    delete game;
}

void MainMenu::on_playMosaicBt_clicked()
{
    Mosaic *game = new Mosaic;
    startGame(game);
    delete game;
}

void MainMenu::on_playMemoryBt_clicked()
{
    Memory *game = new Memory;
    startGame(game);
    delete game;
}

void MainMenu::on_playHangmanBt_clicked()
{
    Hangman *game = new Hangman;
    startGame(game);
    delete game;
}

void MainMenu::on_aboutBt_clicked()
{
    ui->aboutStackedWidget->setCurrentIndex(currentGame);
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainMenu::on_backToMenuBt_clicked()
{
    ui->menuStackedWidget->setCurrentIndex(currentGame);
    ui->mainStackedWidget->setCurrentIndex(0);
}

void MainMenu::on_tableMosaicBt_clicked()
{
    ui->menuStackedWidget->setCurrentIndex(0);
    ui->mainStackedWidget->setCurrentIndex(2);
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
