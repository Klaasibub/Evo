#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QPair>

namespace Ui {
class Memory;
}

class Memory : public QDialog
{
    Q_OBJECT
public:
    int width, height;
    static const QString recordsPath, aboutPath;
    explicit Memory(QWidget *parent = nullptr);
    ~Memory();

private slots:
    void on_gameField_cellClicked(int row, int column);
    void updateTime();

private:
    int amount_dont_opened;
    QTimer *tmr;//Таймер
    int last_wrong;//время после ошибочно увиденной карты
    int game_time;//игровое время
    int** game_field;//игровое поле
    bool** open_cards;//игровое поле
    QPair<int, int> first_coord, second_coord;
    bool is_first_card;//проверка на номер хода игрока
    int points;//количество очков
    Ui::Memory *ui;
    void shuffle_cards();//случайное распределение карточек
    void fill_font();//переворачивание всех карт рубашкой вверх
    void create_game(int row, int col);//создание игрового поля данного размера
    void fill_font_card(int row, int column);//Закрашивание определенной карты

    void check_records();
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);

    void set_widget();
};

#endif // MEMORY_H
