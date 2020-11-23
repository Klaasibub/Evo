#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTableWidget>
#include <QVector>
#include <QMediaPlaylist>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

/*!
 * \brief Класс главного меню игры.
 *
 * Класс главного меню игры, служащий точкой вхождения в игры,
 * а также отображающий таблицу рекордов и информацию об игре.
 */
class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    /// Конструктор класса главного меню.
    MainMenu(QWidget *parent = nullptr);
    /// Деструктор класса главного меню.
    ~MainMenu();
    virtual bool eventFilter(QObject* watched, QEvent* event);

public slots:
    /*!
     * \brief Загрузка страницы игры.
     *
     * Загрузка главной страницы выбранной игры,
     * её таблицы рекордов и информации об игре.
     */
    void loadGamePage();
    /*!
     * \brief Запуск выбранной игры.
     *
     * Запуск игры и скрытие главного меню программы.
     */
    void on_playBt_clicked();
    /*!
     * \brief Загрузка стилей.
     *
     * Считывание qss стилей из файла и применение их к форме.
     */
    void loadStyle();
    /*!
     * \brief Проверка обновлений.
     *
     * Проверка доступности обновлений на сервере.
     */
    void checkUpdate();
    /*!
     * \brief Загрузка таблицы рекордов.
     *
     * Считывание рекордов из файла и загрузка их в таблицу на форме.
     */
    void table();
    /*!
     * \brief Загрузка информации об игре.
     *
     * Считывание информации об игре и её загрузка на форму.
     */
    void about();

private:
    Ui::MainMenu *ui;

    enum class Game{Quiz, Mosaic, Memory, Hangman}; /// Список возможных игр.

    static const int numberGames = 4;   /// Число игр в приложении.
    Game currentGame;                   /// Текущая отображаемая игра.

    QUrl service_url{"http://justyell.pythonanywhere.com"};
    const QString currentVerison = "1.0"; /// Текущая версия приложения
    QMediaPlayer *player_ = nullptr;
    QMediaPlaylist *playlist = nullptr;
    /*!
     * \brief Заполнение таблицы.
     *
     * Заполнение заполнение таблицы данными.
     * \param table - таблица, в которую запишутся данные.
     * \param data - данные для записи.
     */
    void fillTable(QTableWidget *table, QString data);
    void dropAudio();
};
#endif // MAINMENU_H
