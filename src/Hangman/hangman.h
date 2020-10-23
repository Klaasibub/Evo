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
    void on_letter_clicked();
    void showPic(int);
    void startPol();
   // void on_pushButton_show_clicked();

    void on_pushButton_second_clicked();

private:
    Ui::Hangman *ui;
};

#endif // HANGMAN_H
