#ifndef MOSAIC_H
#define MOSAIC_H

#include <QDialog>

namespace Ui {
class Mosaic;
}

class Mosaic : public QDialog
{
    Q_OBJECT

public:
    explicit Mosaic(QWidget *parent = nullptr);
    ~Mosaic();

private:
    Ui::Mosaic *ui;
};

#endif // MOSAIC_H
