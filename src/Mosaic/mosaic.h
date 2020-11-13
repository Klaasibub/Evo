#ifndef MOSAIC_H
#define MOSAIC_H

#include <QDialog>
#include <QTableWidget>
#include <QJsonArray>
#include <QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>

namespace Ui {
class Mosaic;
}
/*!
 * \brief Модуль игры мозаика.
 *
 * Этот модуль содержит основную логику игры "Мозаика".
 */
class Mosaic : public QDialog
{
    Q_OBJECT

public:
    /// Путь до файла с рекордами.
    static const QString recordsPath;
    /// Путь до файла, содержащего информацию об игре.
    static const QString aboutPath;
    /// Конструктор класса Мозаика.
    explicit Mosaic(QWidget *parent = nullptr);
    /// Деструктор класса Мозаика.
    ~Mosaic();
    /*!
     * \brief Загрузка изображения на игровое поле.
     *
     * Считывание данных отдельного изображения и отрисовка его на поле.
     */
    void loadImages();
    /*!
     * \brief Сохранение изображения с поля fieldBig.
     *
     * Считывание RGB-кода ячеек в текстовый файл.
     */
    void writeImage();
    /*!
     * \brief Очистка таблицы.
     *
     * Замена цвета фона всех ячеек на RGB (255,255,255).
     * \param table - таблица для очистки.
     */
    void clearField(QTableWidget *table);
    /*!
     * \brief Загрузка стилей.
     *
     * Считывание QSS-стилей и применение их к форме.
     */
    void loadStyle();
    /*!
     * \brief Проверка на наличие нового рекорда.
     *
     * Проверка того, что очков пользователя
     * достаточно для внесения его в таблицу рекордов.
     */
    void checkTop();

private:
    int step; /// Текущий номер изображения.
    QJsonArray images; /// Массив изображений в формате Json.
    static const int size = 20; /// Размер поля.
    static const int max_records = 20; /// Максимальное число пользователей в таблице рекордов.
    int imagesCount; /// Число картинок.
    QVector<int> vec; /// Порядок вывода изображений.
    Ui::Mosaic *ui;
    QColor currentColor; /// Текущий цвет рисования.
    QTimer *timer; /// Время игры.
    bool paused; /// Флажок паузы.
    QMediaPlayer *player = nullptr; /// Плеер для озвучки.
    QMediaPlaylist *playlist = nullptr; /// Плейлист.
    /*!
     * \brief Фильтр событий.
     *
     * Фильтрует события, если этот объект был установлен в качестве фильтра событий для наблюдаемого объекта.
     * \param watched - наблюдаемый объект.
     * \param event - событие.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event);
    /*!
     * \brief Функция сравнения двух элементов.
     * \param a - первый элемент стравнения.
     * \param b - второй элемент стравнения.
     * \return результат сравнения двух элементов.
     */
    static bool comp(QPair <QString,QTime> a, QPair <QString, QTime> b);

public slots:
    /*!
     * \brief Обработка нажатия на кнопку выбора цвета.
     *
     * Установка выбранного цвета текущим для рисования.
     */
    void onColorBtClicked();
    /*!
     * \brief Обработка нажатия на ячейку.
     *
     * Обработка нажатия на ячейку в поле для рисовнаия.
     * \param row - индекс строки ячейки.
     * \param column - индекс столбца ячейки.
     */
    void on_fieldBig_cellClicked(int row, int column);
    /*!
     * \brief Проверка результатов.
     *
     * Соотвествие нарисованного изображения и шаблона.
     */
    void checkResults();
    /*!
     * \brief Обработка нажатия на кнопку "Старт".
     *
     * Инициализация и запуск игрового процесса.
     */
    void on_startBt_clicked();
    /*!
     * \brief Обработка нажатия на кнопку "Пауза".
     *
     * Остановк игрового процесса.
     */
    void on_pauseBt_clicked();
    /*!
     * \brief Обработка нажатия на кнопку "Продолжить".
     *
     * Продолжение игрового процесса.
     */
    void on_restartBt_clicked();

private slots:
    void updateTime();
};


#endif // MOSAIC_H
