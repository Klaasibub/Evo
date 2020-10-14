#include "quiz.h"
#include "ui_quiz.h"

Quiz::Quiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Quiz)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Quiz::~Quiz()
{
    delete ui;
}
