#include "mosaic.h"
#include "ui_mosaic.h"
#include "QMessageBox"
#include <utils.h>
#include <vector>
#include <random>
#include <ctime>

const QString Mosaic::recordsPath = "/static/mosaic_records.csv";
const QString Mosaic::aboutPath = ":/static/mosaic_about.txt";

Mosaic::Mosaic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mosaic)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
    for(auto item : {ui->blueBt,ui->greyBt,ui->oliveBt,ui->greenBt,ui->blackBt, ui->whiteBt,
                     ui->cyanBt,ui->pinkBt,ui->yellowBt,ui->navyBt, ui->brownBt,ui->redBt}){
        connect(item, SIGNAL(clicked()),this, SLOT(on_colorBt_clicked()));
    }
    connect(ui->readyBt, SIGNAL(clicked()),this,SLOT(checkResults()));

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
            image[i][j]=QColor(0,0,0);
            ui->fieldBig->setItem(i,j,new QTableWidgetItem());
            ui->fieldSmall->setItem(i,j,new QTableWidgetItem());
        }
    }

    //clearField(ui->fieldBig);
    //clearField(ui->fieldSmall);

    ui->readyBt->setEnabled(false);
    ui->startBt->setEnabled(true);

    QString out;
    utils::read_from_file(":/static/images.txt", out, false);
    QJsonObject json = utils::json_loads(out);

    images = json["Images"].toArray();
    imagesCount = images.size();

//    srand(time(NULL));
//    for(int i = 0; i < imagesCount; i++){
//        vec.push_back(i);
//    }
    //std::random_shuffle(vec.begin(), vec.end());
    //std::random_shuffle(images.begin(), images.end());
}

Mosaic::~Mosaic()
{
    delete ui;
}

void Mosaic::on_colorBt_clicked()
{
    QPushButton* btSender = qobject_cast<QPushButton*>(sender());
    currentColor = btSender->palette().color(QWidget::backgroundRole());
}

void Mosaic::on_fieldBig_cellClicked(int row, int column)
{
    ui->fieldBig->item(row, column)->setBackground(QBrush(currentColor));
}

void Mosaic::checkResults()
{
    for (int i = 0; i < size; i++ ) {
        for (int j = 0; j < size; j++ ) {
            if(ui->fieldSmall->item(i,j)->backgroundColor() != ui->fieldBig->item(i,j)->backgroundColor()){
                QMessageBox mb;
                mb.setText("Ошибка!");
                mb.exec();
                return;
            }
        }
    }
    QMessageBox mb;
    mb.setText("Молодец!");
    mb.exec();
    step+=1;
    loadImages();

/* QJsonObject json;
    QJsonArray arr;

    for (int i = 0; i < size; i++ ) {
        for (int j = 0; j < size; j++ ) {
            QJsonObject point;
            point["r"] = ui->fieldBig->item(i,j)->backgroundColor().red();
            point["g"] = ui->fieldBig->item(i,j)->backgroundColor().green();
            point["b"] = ui->fieldBig->item(i,j)->backgroundColor().blue();
            arr.append(point);
        }
    }
    json["Points"] = arr;
    QString str = utils::json_dumps(json);
    utils::write_to_file("POINTS.txt", str, false);
    loadColors();*/
}
void Mosaic::loadImages()
{
    clearField(ui->fieldBig);
    QJsonObject json = images[step].toObject();
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
    loadImages();
    ui->readyBt->setEnabled(true);
    ui->startBt->setEnabled(false);
}

void Mosaic::clearField(QTableWidget *table)
{

    for (int i = 0; i < size; i++ ) {
        for (int j = 0; j < size; j++ ) {
            table->item(i, j)->setBackground(QBrush(QColor(255,255,255)));
        }
    }
}
