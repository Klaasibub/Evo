#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include "utils.h"
#include <Quiz/quiz.h>
#include <Hangman/hangman.h>
#include <Memory/memory.h>
#include <Mosaic/mosaic.h>
#include <QTextCodec>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    currentGame = Game::Quiz;

    loadStyle();
    check_update();
    loadGamePage();

    for(auto bt: {ui->quizGameBt, ui->memoryGameBt, ui->hangmanGameBt, ui->mosaicGameBt})
        connect(bt, SIGNAL(clicked()), this, SLOT(loadGamePage()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::table()
{
    QString recordsPath = ":/static/table.test"; // ToDo: replace to Game.recordPath();
    switch(currentGame)
    {
    case Game::Quiz:
        //recordsPath = Quiz::recordsPath;
        break;
    case Game::Memory:
        //recordsPath = Memory::recordsPath;
        break;
    case Game::Hangman:
        //recordsPath = Hangman::recordsPath;
        break;
    case Game::Mosaic:
        //recordsPath = Mosaic::recordsPath;
        break;
    }
    fillTable(ui->recordsTable, recordsPath);
}

void MainMenu::about()
{
    QString aboutPath, recordsPath = ":/static/table.test"; // ToDo: replace to Game.aboutPath();
    switch(currentGame)
    {
    case Game::Quiz:
        aboutPath = "Quiz::aboutPath";
        break;
    case Game::Memory:
        aboutPath = "Memory::aboutPath";
        break;
    case Game::Hangman:
        aboutPath = "Hangman::aboutPath";
        break;
    case Game::Mosaic:
        aboutPath = "Mosaic::aboutPath";
        break;
    }
    QString about;
    utils::read_from_file(recordsPath, about, false);
    ui->aboutGameText->setMarkdown(about+aboutPath);
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

    table->setRowCount(fmin(rowsData.size()-1, 15));

    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        for (int j = 0; j < rowData.size(); j++){
            table->setItem(i-1, j, new QTableWidgetItem(rowData[j]));
        }
    }
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

void MainMenu::loadGamePage(){
    QPushButton *bt = qobject_cast<QPushButton*>(QObject::sender());
    if(bt == ui->quizGameBt)
        currentGame = Game::Quiz;
    else if(bt == ui->memoryGameBt)
        currentGame = Game::Memory;
    else if(bt == ui->hangmanGameBt)
        currentGame = Game::Hangman;
    else if(bt == ui->mosaicGameBt)
        currentGame = Game::Mosaic;

    table();
    about();
}

void MainMenu::loadStyle(){
    QString style;
    utils::read_from_file(":static/style.css", style, false);
    setStyleSheet(style);
}

void MainMenu::on_playBt_clicked()
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

    game->setModal(true);
    setVisible(false);
    game->exec();
    setVisible(true);
    delete game;
}