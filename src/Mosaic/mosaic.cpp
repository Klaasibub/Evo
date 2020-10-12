#include "mosaic.h"
#include "ui_mosaic.h"

Mosaic::Mosaic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mosaic)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Mosaic::~Mosaic()
{
    delete ui;
}
