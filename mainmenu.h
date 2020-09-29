#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

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

private:
    Ui::MainMenu *ui;
    int currentGame;                    // currently displayed game
    static const int numberGames = 4;   // number of games in the app
};
#endif // MAINMENU_H
