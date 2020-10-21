#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <QTime>
#include <QTimer>

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
    int amount_opened;
    QTimer *tmr;//Таймер
    int last_wrong;//время после ошибочно увиденной карты
    int game_time;//игровое время
    int** game_field;//игровое поле
    int x_prev_coord, y_prev_coord, y_tec_coord, x_tec_coord;
    bool is_first_card;//проверка на номер хода игрока
    int points;//количество очков
    Ui::Memory *ui;
    void shuffle_cards();//случайное распределение карточек
    void fill_font();//переворачивание всех карт рубашкой вверх
    void create_game(int row, int col);//создание игрового поля данного размера
    void fill_font_card(int row, int column);//Закрашивание определенной карты
};

#endif // MEMORY_H
