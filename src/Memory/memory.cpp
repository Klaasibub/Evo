#include "memory.h"
#include "ui_memory.h"
#include <utils.h>
#include <vector>
#include <algorithm>
#include <random>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>
#include <QInputDialog>
#include <QLineEdit>

const QString Memory::recordsPath = "static/memory_records.csv";
const QString Memory::aboutPath = ":/Memory/about.txt";
const int time_after_wrong_hod = 1;

static QBrush* brush;
static QBrush brush_font;

Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->create_game(4, 5);
    shuffle_cards();
    brush = new QBrush[10];

    brush_font.setTextureImage(QImage(":/Memory/font"));
    for(int i = 0; i < 10; i++){
        brush[i].setTextureImage(QImage(":/Memory/" + QString::number(i)));
    }

    ui->timerLabel->setText(QTime(0, 0, game_time, 0).toString("m:ss"));
    tmr = new QTimer();
    tmr->setInterval(1000);
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
    tmr->start();
    is_first_card = true;
    this->set_widget();
    //Инициализация массива случайными парами
    //Подгрузка рубашки карт
    this->fill_font();
    ui->pointsLabel->setText(QString:: number(points));
}

void Memory:: set_widget(){
    //Настройки QT WidgetTable
    ui->gameField->setColumnCount(this->width);
    ui->gameField->setRowCount(this->height);
    ui->gameField->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gameField->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Загрузка картинок
    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            ui->gameField->setItem(i, j, new QTableWidgetItem());
        }
    }
}

Memory::~Memory()
{
    ui->gameField->clear();
    ui->gameField->setRowCount(0);
    ui->gameField->setColumnCount(0);
    for(int i = 0; i < this->height; i++){
        delete[] game_field[i];
    }
    delete[] game_field;
    delete[] brush;
    delete tmr;
    delete ui;
}

void Memory:: shuffle_cards(){//Перемешивание карт
    int amount = this->width * this->height;
    std:: vector<int> numbers;
    for(int i = 0; i < amount/2; i++){
        numbers.push_back(i);
        numbers.push_back(i);
    }
    std::random_shuffle(std::begin(numbers), std::end(numbers));
    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            game_field[i][j] = numbers[i * this->width + j];
        }
    }
}

void Memory:: fill_font(){//Закраска внешней стороной карточек
    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            if(game_field[i][j] != -1){
                ui->gameField->item(i, j)->setBackground(brush_font);
            }
            else{
                ui->gameField->item(i, j)->setBackground(brush[i]);
            }
        }
    }
}

void Memory:: create_game(int row, int col){//Случайное заполнение игрового поля
    this->amount_dont_opened = row * col / 2;
    this->points = 0;
    this->last_wrong = -10;
    this->game_time = 0;
    this->width = col;// w = 5;
    this->height = row;// h = 4;
    this->game_field = new int*[this->width];
    for(int i = 0; i < this->height; i ++){
        this->game_field[i] = new int[this->width];
    }
}

void Memory::on_gameField_cellClicked(int row, int column)
{
    if(game_field[row][column] == -1 || game_time - last_wrong < time_after_wrong_hod){
        return;
    }
    else if(is_first_card == true){
        first_coord = qMakePair(row, column);
        is_first_card = !is_first_card;
        QBrush brush;
        brush.setTextureImage(QImage(":/Memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
        return;
    }
    else if(first_coord == qMakePair(row, column)){
        return;
    }
    //Ход сделан неверно
    else if(game_field[row][column] != game_field[first_coord.first][first_coord.second]){
        QBrush brush;
        brush.setTextureImage(QImage(":/Memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
        second_coord = qMakePair(row, column);
        last_wrong = game_time;
        is_first_card = !is_first_card;
        if(points > 0){
            points--;
        }
    }
    //Ход сделан верно
    else{
        QBrush brush;
        brush.setTextureImage(QImage(":/Memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
        points += 10;
        game_field[row][column] = -1;
        game_field[first_coord.first][first_coord.second] = -1;
        is_first_card = !is_first_card;
        amount_dont_opened--;
        if(amount_dont_opened == 0){
            ui->pointsLabel->setText(QString:: number(points));
            QMessageBox::information(this, "You won!", "Your score: " + QString:: QString:: number(this->points));
            check_records();
            //Завершаем старую игру и начинаем новую!!!!!!
            close();
        }
    }
    ui->pointsLabel->setText(QString:: number(points));
}

void Memory:: fill_font_card(int row, int column){
    QBrush brush;
    brush.setTextureImage(QImage(":/Memory/font"));
    ui->gameField->item(row, column)->setBackground(brush);
}

void Memory:: updateTime()
{
    //Сообщение о проигрыше при переполнении времени, после часа игры
    game_time++;
    ui->timerLabel->setText(QTime(0, game_time / 60, game_time % 60, 0).toString("m:ss"));
    if(game_time - last_wrong == time_after_wrong_hod){
        fill_font_card(first_coord.first, first_coord.second);
        fill_font_card(second_coord.first, second_coord.second);
    }
}

void Memory::check_records(){
    QVector<QPair<QString, int>> records;
    QString data;
    QString record_path = QDir::currentPath() + "/" + Memory::recordsPath;
    utils::read_from_file(record_path, data, false);
    QStringList rowData, rowsData = data.split("\n");

    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        if (rowData.size()>1){
            records.push_back(QPair<QString, int>(rowData[0], rowData[1].toInt()));
        }
    }
    std::sort(records.begin(),records.end(), this->comp);

    if (records.size()<20 || records[19].second< this->points){
        bool bOk;
        QString str = QInputDialog::getText( 0,
                                             "Введите имя",
                                             "Ваше имя:",
                                             QLineEdit::Normal,
                                             "",
                                             &bOk
                                            );
        if (bOk) {
            if (records.size()<20){
                records.append(QPair<QString, int>(str, this->points));
            }
            else{
                 records[19].first = str;
                 records[19].second = this->points;
            }
        }
    }

    std::sort(records.begin(),records.end(),this->comp);

    data = "Nickname;Score\n";

    for(auto i: records){
        data += i.first + ";" + QString::number(i.second) + "\n";
    }

    utils::write_to_file(record_path, data, false);
}

bool Memory::comp (QPair <QString, int > a, QPair <QString, int > b) {
  return a.second > b.second;
}
