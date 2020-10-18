#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void previousGame();
    void nextGame();
    void exitGame();
    void startGame(QDialog *game);

    void play();
    void about();
    void table();
    void backToMenu();

    void check_update();

    void fillTable(QTableWidget *table, QString data);

private:
    Ui::MainMenu *ui;

    enum class MainWidget{Menu, About, Table};
    enum class Game{Quiz, Mosaic, Memory, Hangman};
    static const int numberGames = 4;   // number of games in the app
    Game currentGame;                   // currently displayed game

    QUrl service_url{"http://justyell.pythonanywhere.com"};
    const QString currentVerison = "1.0";
};
#endif // MAINMENU_H
