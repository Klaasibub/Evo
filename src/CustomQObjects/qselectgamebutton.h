#ifndef SELECTGAMEBUTTON_H
#define SELECTGAMEBUTTON_H

#include <QPushButton>

class QSelectGameButton: public QPushButton
{
    Q_OBJECT

public:
    explicit QSelectGameButton(QWidget *parent = nullptr): QPushButton(parent) { }
    void setNumberGame(int gameNumber){ this->gameNumber = gameNumber; }
    int getNumberGame(){ return gameNumber; }

private slots:

private:
    int gameNumber;
};


#endif // SELECTGAMEBUTTON_H
