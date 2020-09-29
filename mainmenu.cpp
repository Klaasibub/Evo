#include "mainmenu.h"
#include "./ui_mainmenu.h"
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

    connect(ui->leftMillionaireBt, SIGNAL(clicked()),this,SLOT(previousGame()));
    connect(ui->leftHangmanBt, SIGNAL(clicked()),this,SLOT(previousGame()));
    connect(ui->leftMosaicBt, SIGNAL(clicked()),this,SLOT(previousGame()));
    connect(ui->leftMemoryBt, SIGNAL(clicked()),this,SLOT(previousGame()));

    connect(ui->rightMillionaireBt, SIGNAL(clicked()),this,SLOT(nextGame()));
    connect(ui->rightHangmanBt, SIGNAL(clicked()),this,SLOT(nextGame()));
    connect(ui->rightMosaicBt, SIGNAL(clicked()),this,SLOT(nextGame()));
    connect(ui->rightMemoryBt, SIGNAL(clicked()),this,SLOT(nextGame()));

    connect(ui->exitMillionaireBt, SIGNAL(clicked()),this,SLOT(exitGame()));
    connect(ui->exitHangmanBt, SIGNAL(clicked()),this,SLOT(exitGame()));
    connect(ui->exitMosaicBt, SIGNAL(clicked()),this,SLOT(exitGame()));
    connect(ui->exitMemoryBt, SIGNAL(clicked()),this,SLOT(exitGame()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::previousGame()
{
    currentGame = (numberGames+currentGame-1)%numberGames;
    ui->stackedWidget->setCurrentIndex(currentGame);
}

void MainMenu::nextGame()
{
    currentGame = (currentGame+1)%numberGames;
    ui->stackedWidget->setCurrentIndex(currentGame);
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
