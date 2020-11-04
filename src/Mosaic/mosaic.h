#ifndef MOSAIC_H
#define MOSAIC_H

#include <QDialog>
#include <QTableWidget>
#include <QJsonArray>

namespace Ui {
class Mosaic;
}

class Mosaic : public QDialog
{
    Q_OBJECT

public:
    static const QString recordsPath;
    static const QString aboutPath;
    explicit Mosaic(QWidget *parent = nullptr);
    ~Mosaic();

private:
    int step;
    QJsonArray images;
    static const int size = 20;
    int imagesCount;
    QVector<int> vec;
    Ui::Mosaic *ui;
    QColor currentColor;
    QColor image[size][size];
    void loadImages();
    void clearField(QTableWidget *table);

private slots:
    void on_colorBt_clicked();
    void on_fieldBig_cellClicked(int row, int column);
    void checkResults();
    void on_startBt_clicked();
};

#endif // MOSAIC_H
