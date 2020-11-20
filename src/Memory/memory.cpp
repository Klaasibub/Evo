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
const QString Memory::savePath = "/Memory/save.txt";
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
    if(this->load_game() != 0){
        this->create_game(4, 5);
        this->shuffle_cards();
    }
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
        delete[] open_cards[i];
    }
    delete[] game_field;
    delete[] open_cards;
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
            if(open_cards[i][j] == false){
                ui->gameField->item(i, j)->setBackground(brush_font);
            }
            else{
                ui->gameField->item(i, j)->setBackground(brush[game_field[i][j]]);
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
    this->game_field = new int*[this->height];
    this->open_cards = new bool*[this->height];
    for(int i = 0; i < this->height; i ++){
        this->game_field[i] = new int[this->width];
        this->open_cards[i] = new bool[this->width]();//Заполняем сразу же ложью
    }
}

void Memory::on_gameField_cellClicked(int row, int column)
{
    if(open_cards[row][column] == true || game_time - last_wrong < time_after_wrong_hod){
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
        open_cards[row][column] = true;
        open_cards[first_coord.first][first_coord.second] = true;
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
    this->save_game();
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

int Memory:: save_game(){
    QString result_string;
    QString saveFilePath = QDir:: currentPath() + "/" + Memory::savePath;
    if(amount_dont_opened == 0){
        //Удаление файла
        QDir file;
        file.remove(saveFilePath);
        return -1;
    }
    else{
        result_string += QString::number(amount_dont_opened).rightJustified(3, '0');
        //Запись информации о времени
        result_string += QString::number(game_time).rightJustified(3, '0');
        //Points
        result_string += QString::number(points).rightJustified(3, '0');
        //width,height
        result_string += QString::number(width).rightJustified(3, '0');
        result_string += QString::number(height).rightJustified(3, '0');
        //game_field
        for(int i = 0; i < this->height; i++){
            for(int j = 0; j < this->width; j++){
                result_string += QString::number(game_field[i][j]) + "|";
            }
        }
        result_string += "!";
        //open_cards
        for(int i = 0; i < this->height; i++){
            for(int j = 0; j < this->width; j++){
                result_string += QString::number(open_cards[i][j]) + "|";
            }
        }
        if(utils:: write_to_file(saveFilePath, result_string, true) != 0){
            return 1;
        }
        return 0;
    }
}

int Memory:: load_game(){
    QString result_string;
    QString loadFilePath = QDir:: currentPath() + "/" + Memory::savePath;
    if(utils::read_from_file(loadFilePath, result_string, true) != 0 ){
        //Игра не открылась или не существует
        return 1;
    }
    else{
        //Записаны сохраненные данные
        this->amount_dont_opened = result_string.midRef(0, 3).toInt();
        this->game_time = result_string.midRef(3, 3).toInt();
        this->points = result_string.midRef(6, 3).toInt();
        this->width = result_string.midRef(9, 3).toInt();
        this->height = result_string.midRef(12, 3).toInt();
        this->last_wrong = -10;
        this->open_cards = new bool*[this->height];
        this->game_field = new int*[this->width];
        for(int i = 0; i < this->height; i ++){
            this->game_field[i] = new int[this->width];
            this->open_cards[i] = new bool[this->width];
        }
        QStringRef str_game_field = result_string.midRef(15, result_string.size() - 15);
        QVector<QStringRef> game_stats = str_game_field.split(QLatin1Char('!'), Qt::SkipEmptyParts);
        QVector<QStringRef> vect_game_field, vect_open_cards;
        vect_game_field = game_stats[0].split(QLatin1Char('|'), Qt::SkipEmptyParts);
        vect_open_cards = game_stats[1].split(QLatin1Char('|'), Qt::SkipEmptyParts);
        for(int i = 0 ; i < this->height; i++){
            for(int j = 0; j < this->width; j++){
                game_field[i][j] = (vect_game_field[i * this->width + j]).toInt();
                open_cards[i][j] = (vect_open_cards[i * this->width + j]).toInt();
            }
        }
    }
    int a = 5;
    return 0;
}
