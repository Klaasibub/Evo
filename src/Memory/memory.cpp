#include "memory.h"
#include "ui_memory.h"
#include <vector>
#include <algorithm>
#include <random>
#include <QMessageBox>

const QString recordsPath = "static/memory_records.csv";
const QString aboutPath = ":/static/about_memory.txt";
const int time_after_wrong_hod = 1;
//int w, h;
Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
    tmr = new QTimer();
    tmr->setInterval(1000);
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
    tmr->start();
    is_first_card = true;
    this->create_game(4, 5);
    //Настройки QT WidgetTable
    ui->gameField->setColumnCount(this->width);
    ui->gameField->setRowCount(this->height);
    ui->gameField->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gameField->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gameField->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gameField->setSelectionMode(QAbstractItemView::NoSelection);
    ui->gameField->setFocusPolicy(Qt::NoFocus);
    ui->gameField->verticalHeader()->setVisible(false);
    ui->gameField->horizontalHeader()->setVisible(false);
    //Загрузка картинок
    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            ui->gameField->setItem(i, j, new QTableWidgetItem());
        }
    }
    //Инициализация массива случайными парами
    shuffle_cards();
    //Подгрузка рубашки карт
    fill_font();
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
    QBrush brush;
    brush.setTextureImage(QImage(":/memory/font"));
    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            ui->gameField->item(i, j)->setBackground(brush);
        }
    }
}

void Memory:: create_game(int row, int col){//Случайное заполнение игрового поля
    this->amount_opened = 0;
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
    else if(is_first_card){
        y_prev_coord = row;
        x_prev_coord = column;
        is_first_card = !is_first_card;
        QBrush brush;
        brush.setTextureImage(QImage(":/memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
    }
    else if(game_field[row][column] != game_field[y_prev_coord][x_prev_coord]){
        QBrush brush;
        brush.setTextureImage(QImage(":/memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
        y_tec_coord = row;
        x_tec_coord = column;
        last_wrong = game_time;
        is_first_card = !is_first_card;
        points--;
    }
    else{
        QBrush brush;
        brush.setTextureImage(QImage(":/memory/" + QString::number(game_field[row][column])));
        ui->gameField->item(row, column)->setBackground(brush);
        points += 10;
        game_field[row][column] = -1;
        game_field[y_prev_coord][x_prev_coord] = -1;
        is_first_card = !is_first_card;
        amount_opened += 2;
        if(amount_opened == this->width * this->height){
            QMessageBox::information(this, "You won!", "Your score: " + QString:: QString:: number(this->points));
        }
    }
    ui->pointsLabel->setText(QString:: number(points));
}

void Memory:: fill_font_card(int row, int column){
    QBrush brush;
    brush.setTextureImage(QImage(":/memory/font"));
    ui->gameField->item(row, column)->setBackground(brush);
}

void Memory:: updateTime()
{
    game_time++;
    ui->timerLabel->setText(QTime(0, 0, game_time, 0).toString("m:ss"));
    if(game_time - last_wrong == time_after_wrong_hod){
        fill_font_card(y_prev_coord, x_prev_coord);
        fill_font_card(y_tec_coord, x_tec_coord);
    }
}
