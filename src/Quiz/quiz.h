#ifndef QUIZ_H
#define QUIZ_H

#include <QRadioButton>
#include <QDialog>
#include <QJsonObject>
#include <QTimer>
#include <QTime>

namespace Ui {
class Quiz;
}

class Quiz : public QDialog
{
    Q_OBJECT

public:
    static const QString recordsPath, aboutPath;
    static const int maxQuestions = 27;//максимальное число вопросов
    explicit Quiz(QWidget *parent = nullptr);
    ~Quiz();


private slots:
    void on_Fifty_clicked();

    void on_Chance_clicked();

    void on_Hundred_clicked();

    void on_rb_clicked();


private:
    Ui::Quiz *ui;
    int orderQuestions[maxQuestions];
    bool fifty, hundred, chance, isUsedChanse, gameOver;
    int question, questionID;
    QJsonObject questionsJson;
    QString rightAnswer;

    void fillNumberQuestions();
    bool loadQuestionByNum(int num_question);
    void initNewGame();
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);
    void check_records();

};

#endif // QUIZ_H
