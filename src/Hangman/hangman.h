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
    explicit Hangman(QWidget *parent = nullptr);
    ~Hangman();

private:
    Ui::Hangman *ui;
};

#endif // HANGMAN_H
