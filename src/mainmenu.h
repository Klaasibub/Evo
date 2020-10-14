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
    void startGame(QDialog *game);

    void on_playQuizBt_clicked();

    void on_playMosaicBt_clicked();

    void on_playMemoryBt_clicked();

    void on_playHangmanBt_clicked();

    void on_aboutBt_clicked();

    void on_backToMenuBt_clicked();

    void on_tableMosaicBt_clicked();

private:
    Ui::MainMenu *ui;
    int currentGame;                    // currently displayed game
    static const int numberGames = 4;   // number of games in the app
};
#endif // MAINMENU_H
