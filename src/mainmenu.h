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
    void loadGamePage();

    void on_playBt_clicked();

private:
    Ui::MainMenu *ui;

    enum class MainWidget{Menu, About, Table};
    enum class Game{Quiz, Mosaic, Memory, Hangman};

    static const int numberGames = 4;   // number of games in the app
    Game currentGame;                   // currently displayed game

    QUrl service_url{"http://justyell.pythonanywhere.com"};
    const QString currentVerison = "1.0";

    void loadStyle();
    void check_update();
    void table();
    void about();
    void fillTable(QTableWidget *table, QString data);

};
#endif // MAINMENU_H
