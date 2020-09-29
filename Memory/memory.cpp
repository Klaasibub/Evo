#include "memory.h"
#include "ui_memory.h"

Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Memory::~Memory()
{
    delete ui;
}
