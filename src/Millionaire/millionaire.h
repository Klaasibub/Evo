#ifndef MILLIONAIRE_H
#define MILLIONAIRE_H

#include <QDialog>

namespace Ui {
class Millionaire;
}

class Millionaire : public QDialog
{
    Q_OBJECT

public:
    explicit Millionaire(QWidget *parent = nullptr);
    ~Millionaire();

private:
    Ui::Millionaire *ui;
};

#endif // MILLIONAIRE_H
