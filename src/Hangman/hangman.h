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
    void check_records();
    void setStartData(QString);
    void setStyle(QString);
    void showPic(int);
    void startPol();
    void showLetter();

private slots:
    void onLetterClicked();
    void on_pushButton_show_clicked();
    void on_pushButton_second_clicked();
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);

private:
    Ui::Hangman *ui;
    QStringList words;
    QString word_on_screen;
    int score;
    int score_m;
    int mistakes;
    int current_word;
};

#endif // HANGMAN_H
