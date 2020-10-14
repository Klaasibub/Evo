#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include "utils.h"
#include <Millionaire/millionaire.h>
#include <Hangman/hangman.h>
#include <Memory/memory.h>
#include <Mosaic/mosaic.h>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    currentGame = 0;

    for(auto item : {ui->leftMillionaireBt, ui->leftHangmanBt, ui->leftMosaicBt, ui->leftMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(previousGame()));

    for(auto item : {ui->rightMillionaireBt, ui->rightHangmanBt, ui->rightMosaicBt, ui->rightMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(nextGame()));

    for(auto item : {ui->aboutMillionaireBt, ui->aboutHangmanBt, ui->aboutMosaicBt, ui->aboutMemoryBt})
        connect(item, SIGNAL(clicked()),this,SLOT(on_aboutBt_clicked()));

    for(auto item : {ui->backAboutMillionaireBt, ui->backAboutHangmanBt, ui->backAboutMosaicBt, ui->backAboutMemoryBt,
                     ui->backTableMillionaireBt, })
        connect(item, SIGNAL(clicked()),this,SLOT(on_backToMenuBt_clicked()));

    for(auto item : {ui->exitMillionaireBt, ui->exitHangmanBt, ui->exitMosaicBt, ui->exitMemoryBt})
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

void MainMenu::on_playMillionaireBt_clicked()
{
    Millionaire *game = new Millionaire;
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
