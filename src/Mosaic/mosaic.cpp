#include "mosaic.h"
#include "ui_mosaic.h"
#include "QMessageBox"
#include <utils.h>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <QTime>
#include <QDir>
#include <QTextCodec>
#include <QInputDialog>

const QString Mosaic::recordsPath = "/static/records_mosaic.csv";
const QString Mosaic::aboutPath = ":/Mosaic/about.txt";
Mosaic::Mosaic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mosaic)
{
    ui->setupUi(this);
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    loadStyle();
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
    for(auto item : {ui->blueBt,ui->greyBt,ui->oliveBt,ui->greenBt,ui->blackBt, ui->whiteBt,
                     ui->cyanBt,ui->pinkBt,ui->yellowBt,ui->navyBt, ui->brownBt,ui->redBt}){
        connect(item, SIGNAL(clicked()),this, SLOT(onColorBtClicked()));
        item->installEventFilter(this);
    }
    connect(ui->readyBt, SIGNAL(clicked()),this,SLOT(checkResults()));
    ui->readyBt->installEventFilter(this);
    ui->pauseBt->installEventFilter(this);
    ui->startBt->installEventFilter(this);
    ui->restartBt->installEventFilter(this);

    //заполнение матрицы цветами
    currentColor = QColor(255,255,255);
    ui->fieldBig->setColumnCount(size);
    ui->fieldBig->setRowCount(size);
    ui->fieldSmall->setColumnCount(size);
    ui->fieldSmall->setRowCount(size);
    for (int i = 0; i < size; i++ ) {
        ui->fieldBig->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->fieldSmall->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->fieldBig->verticalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->fieldSmall->verticalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        for (int j = 0; j < size; j++ ) {
            ui->fieldBig->setItem(i,j,new QTableWidgetItem());
            ui->fieldSmall->setItem(i,j,new QTableWidgetItem());
        }
    }
    ui->fieldBig->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fieldSmall->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->timeEdit->setEnabled(false);

    clearField(ui->fieldBig);
    clearField(ui->fieldSmall);

    ui->readyBt->setEnabled(false);
    ui->startBt->setEnabled(true);
    ui->pauseBt->setEnabled(false);
    ui->restartBt->setEnabled(false);

    readImages();

    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    paused = true;
}

Mosaic::~Mosaic()
{
    if (player){
        player->stop();
        player->deleteLater();
        player = nullptr;
    }

    if (playlist){
        playlist->clear();
        playlist = nullptr;
    }

    delete timer;
    delete ui;
}

void Mosaic::loadStyle(){
    QString style;
    utils::read_from_file(":/Mosaic/style.css", style, false);
    setStyleSheet(style);
}

void Mosaic::onColorBtClicked()
{
    QPushButton* btSender = qobject_cast<QPushButton*>(sender());
    currentColor = btSender->palette().color(QWidget::backgroundRole());
}

void Mosaic::on_fieldBig_cellClicked(int row, int column)
{
    if(paused)
        return;
    ui->fieldBig->item(row, column)->setBackground(QBrush(currentColor));
}
bool Mosaic::compareTables(QTableWidget *field1, QTableWidget *field2)
{
    if (field1->rowCount() != field2->rowCount() || field1->columnCount() != field2->columnCount()){
        return false;
    }
    for (int i = 0; i < field2->rowCount(); i++ ) {
        for (int j = 0; j < field2->columnCount(); j++ ) {
            if(field1->item(i,j)->background() != field2->item(i,j)->background()){
                return false;
            }
        }
    }
    return true;
}
void Mosaic::checkResults()
{
    if (compareTables(ui->fieldSmall,ui->fieldBig) == true){
        step+=1;
        if(step == imagesCount){
            QMessageBox mb;
            mb.setText("Игра окончена!");
            mb.exec();
            checkTop();
            close();
        }
        else{
            QMessageBox mb;
            mb.setText("Молодец!");
            mb.exec();
            loadImage();
        }
    }
    else{
        QMessageBox mb;
        mb.setText("Ошибка!");
        mb.exec();
    }
}

void Mosaic::writeImage()
{
    QJsonObject json;
    QJsonArray arr;

    for (int i = 0; i < size; i++ ) {
        for (int j = 0; j < size; j++ ) {
            QJsonObject point;
            point["r"] = ui->fieldBig->item(i,j)->background().color().red();
            point["g"] = ui->fieldBig->item(i,j)->background().color().red();
            point["b"] = ui->fieldBig->item(i,j)->background().color().red();
            arr.append(point);
        }
    }
    json["Points"] = arr;
    QString str = utils::json_dumps(json);
    utils::write_to_file(QDir::currentPath() + "/POINTS.txt", str, false);
}

void Mosaic::readImages()
{
    QString out;
    utils::read_from_file(":/Mosaic/images.txt", out, false);
    QJsonObject json = utils::json_loads(out);

    images = json["Images"].toArray();
    imagesCount = images.size();

    srand(time(NULL));
    for(int i = 0; i < imagesCount; i++){
        vec.push_back(i);
    }
    std::random_shuffle(vec.begin(), vec.end());
}

void Mosaic::loadImage()
{
    clearField(ui->fieldBig);
    if(imagesCount == 0)
        return;
    QJsonObject json = images[vec[step]].toObject();
    int c = 0;
    for (int i = 0; i < size; i++ ) {
        for (int j = 0; j < size; j++ ) {
            QJsonObject colorJson = json["Points"].toArray()[c++].toObject();
            QColor color(colorJson["r"].toInt(), colorJson["g"].toInt(), colorJson["b"].toInt());
            ui->fieldSmall->item(i, j)->setBackground(QBrush(color));
        }
    }
}

void Mosaic::on_startBt_clicked()
{
    step = 0;
    loadImage();
    ui->readyBt->setEnabled(true);
    ui->startBt->setEnabled(false);
    ui->pauseBt->setEnabled(true);
    timer->start();
    paused = false;
}

void Mosaic::clearField(QTableWidget *table)
{
    for (int i = 0; i < table->rowCount(); i++ ) {
        for (int j = 0; j < table->columnCount(); j++ ) {
            table->item(i, j)->setBackground(QBrush(QColor(255,255,255)));
        }
    }
}

void Mosaic::updateTime()
{
    ui->timeEdit->setTime(QTime(ui->timeEdit->time().addSecs(1)));
}

void Mosaic::on_pauseBt_clicked()
{
    timer->stop();
    ui->restartBt->setEnabled(true);
    ui->pauseBt->setEnabled(false);
    paused = true;
}

void Mosaic::on_restartBt_clicked()
{
    timer->start();
    ui->restartBt->setEnabled(false);
    ui->pauseBt->setEnabled(true);
    paused = false;
}

void Mosaic::checkTop()
{
    QVector<QPair<QString,QTime>> records;
    QString data;
    QTime time = ui->timeEdit->time();
    QString record_path = QDir::currentPath() + "/" + Mosaic::recordsPath;
    utils::read_from_file(record_path, data, false);
    QStringList rowData, rowsData = data.split("\n");

    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        if (rowData.size()>1){
            records.push_back(QPair<QString, QTime>(rowData[0], QTime::fromString(rowData[1],"mm:ss")));
        }
    }
    std::sort(records.begin(),records.end(),this->comp);

    if (records.size() < max_records || records[max_records-1].second > time){
        bool bOk;
        fireworkAudio();
        QString str = QInputDialog::getText(this, "Новый рекорд!", "Введите свой ник:", QLineEdit::Normal, "", &bOk);
        if (bOk) {
            if (records.size() < max_records){
                records.append(QPair<QString, QTime>(str,time));
            }
            else{
                 records[max_records-1].first = str;
                 records[max_records-1].second = time;
            }
        }
    }

    std::sort(records.begin(),records.end(),this->comp);

    data = "Nickname;Score\n";

    for(auto i: records){
        data += i.first + ";" + i.second.toString("mm:ss") + "\n";
    }

    utils::write_to_file(record_path, data, false);
}

bool Mosaic::comp (QPair <QString,QTime> a, QPair <QString, QTime> b) {
  return a.second < b.second;
}

bool Mosaic::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::HoverEnter && qobject_cast <QPushButton*>(watched)->isEnabled())
    {
        if (!player){
            player = new QMediaPlayer(this);
        }
        if (!playlist){
            playlist = new QMediaPlaylist(this);
            playlist->addMedia(QUrl("qrc:/default_hover_button.mp3"));
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        }
        player->setPlaylist(playlist);
        player->setVolume(10);
        player->play();
    }
    return QDialog::eventFilter(watched, event);
}

void Mosaic::fireworkAudio()
{
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/end_of_game.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player->setPlaylist(playlist);
    player->setVolume(10);
    player->play();
}

QColor Mosaic::getCurrentColor()
{
    return currentColor;
}

bool Mosaic::gameNotOnPause()
{
    return timer->isActive();
}
