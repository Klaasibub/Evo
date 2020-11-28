#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QPair>

namespace Ui {
class Memory;
}
/*!
 * \brief Модуль игры Память.
 *
 * Этот модуль содержит основную логику игры "Память".
 */
class Memory : public QDialog
{
    Q_OBJECT
public:
    ///Путь до файла с рекордами
    static const QString recordsPath;
    ///Путь до файла, содержащего информацию об игре
    static const QString aboutPath;
    ///Путь до файла содержащего сохраненную игру
    static const QString savePath;
    ///Конструктор класса Память
    explicit Memory(QWidget *parent = nullptr);
    ///Деструктор класса память
    ~Memory();
    /*!
     * \brief Сохранение игры
     *
     * \return - 0 при удачном сохранении, иначе ошибка
     */
    int save_game();
    /*!
     * \brief Загрузка текущей игры
     *
     * \return - 0 при удачной загрузке, иначе ошибка
     */
    int load_game();
    /*!
     * \brief Создание игры
     *
     * Создание игрового поля заданного размера со всеми входными данными
     * \param row - Количество строк
     * \param col - Количество столбцов
     */
    void create_game(int row, int col);

public slots:
    /*!
     * \brief Обработчик нажатия на ячейку игрового поля
     *
     * \param row - индекс нажатой строки
     * \param column - индекс нажатого столбца
     */
    void on_gameField_cellClicked(int row, int column);
    /*!
     * \brief функция таймера
     * Таймер, выполняется раз в time, увеличивает счетчик времени
     */
    void updateTime();

private:
    ///Ширина игрового поля
    int width;
    ///Высота игрового поля
    int height;
    ///Счетчик оставшихся неоткрытыми игровых полей
    int amount_dont_opened;
    ///Указатель на таймер
    QTimer *tmr;
    ///Время после ошибочно увиденной карты
    int last_wrong;
    ///Игровое время
    int game_time;
    ///Игровое поле
    int** game_field;
    ///Игровое поле
    bool** open_cards;
    ///Координаты первой карты из пары выбранных
    QPair<int, int> first_coord;
    ///Координаты второй карты из пары выбранных
    QPair<int, int> second_coord;
    ///Проверка номера выбираемой карты(первая или вторая)
    bool is_first_card;
    ///Количество очков
    int points;
    Ui::Memory *ui;
    /*!
     * \brief случайное раскладывание карт по игровому полю
     *
     * Функция случайно распределяет по полю N пар карт
     */
    void shuffle_cards();
    /*!
     * \brief переворачивание всех карт рубашкой вверх
     *
     * Закрашивание полей фоновым рисунком
     */
    void fill_font();
    /*!
     * \brief Открывание карты
     *
     * Закрашивание карты соответствующим ей изображением
     * \param row - Индекс строки
     * \param column - Индекс столбца
     */
    void fill_font_card(int row, int column);
    /*!
     * \brief Проверка рекорда текущей игры
     *
     * Просмотр сущствующих рекордов и сравнение с текущим счетом, получение новой таблицы рекордов
     */
    void check_records();
    /*!
     * \brief Лексиграфическое сравнение 2-х строчек
     *
     * \param a - Первая строка
     * \param b - Вторая строка
     * \return - Логическая переменная a>b
     */
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);
    /*!
     * \brief Установка параметров игрового окна
     */
    void set_widget();
};

#endif // MEMORY_H

