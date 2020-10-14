#ifndef QUIZ_H
#define QUIZ_H

#include <QDialog>

namespace Ui {
class Quiz;
}

class Quiz : public QDialog
{
    Q_OBJECT

public:
    explicit Quiz(QWidget *parent = nullptr);
    ~Quiz();

private:
    Ui::Quiz *ui;
};

#endif // QUIZ_H
