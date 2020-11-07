#ifndef MOSAIC_H
#define MOSAIC_H

#include <QDialog>
#include <QTableWidget>
#include <QJsonArray>
#include <QTimer>

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
    QTimer *timer;
    bool paused;
    void loadImages();
    void writeImage();
    void clearField(QTableWidget *table);
    void loadStyle();
    void checkTop();
    static bool comp(QPair <QString,QTime> a, QPair <QString, QTime> b);

private slots:
    void onColorBtClicked();
    void on_fieldBig_cellClicked(int row, int column);
    void checkResults();
    void on_startBt_clicked();
    void updateTime();
    void on_pauseBt_clicked();
    void on_restartBt_clicked();
};

#endif // MOSAIC_H
