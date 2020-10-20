#ifndef QUIZ_H
#define QUIZ_H

#include <QRadioButton>
#include <QDialog>
#include <QJsonObject>

namespace Ui {
class Quiz;
}

class Quiz : public QDialog
{
    Q_OBJECT

public:
    static const int maxQuestions = 12;//максимальное число вопросов
    explicit Quiz(QWidget *parent = nullptr);
    ~Quiz();


private slots:
    void on_Fifty_clicked();

    void on_Chance_clicked();

    void on_Hundred_clicked();

    void on_rb_clicked();

private:
    Ui::Quiz *ui;
    const int points[maxQuestions]={ 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000, 250000, 500000, 1000000 };
    int orderQuestions[maxQuestions];
    bool fifty, hundred, chance, isUsedChanse, gameOver;
    int question, questionID;
    QJsonObject questionsJson;
    QString rightAnswer;

    void fillNumberQuestions();
    QRadioButton* getRadioButton(int i);
    bool loadQuestionByNum(int num_question);
    void initNewGame();

};

#endif // QUIZ_H
