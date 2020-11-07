#ifndef HANGMAN_H
#define HANGMAN_H

#include <QDialog>

namespace Ui {
class Hangman;
}

class Hangman : public QDialog
{
    Q_OBJECT

public:
    static const QString recordsPath, aboutPath;
    explicit Hangman(QWidget *parent = nullptr);
    ~Hangman();

private slots:
    void onLetterClicked();
    void showPic(int);
    void startPol();
    void on_pushButton_show_clicked();
    void on_pushButton_second_clicked();
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);
    void check_records();
private:
    Ui::Hangman *ui;
};

#endif // HANGMAN_H
