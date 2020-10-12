#include "hangman.h"
#include "ui_hangman.h"

Hangman::Hangman(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Hangman)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Hangman::~Hangman()
{
    delete ui;
}
