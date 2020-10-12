#include "millionaire.h"
#include "ui_millionaire.h"

Millionaire::Millionaire(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Millionaire)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Millionaire::~Millionaire()
{
    delete ui;
}
