#include "quiz.h"
#include "ui_quiz.h"
#include <random>
#include <QTimer>
#include <utils.h>
#include <QMessageBox>

Quiz::Quiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Quiz)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for(auto item : {ui->radioButton, ui->radioButton_2, ui->radioButton_3, ui->radioButton_4})
        connect(item, SIGNAL(clicked()),this,SLOT(on_rb_clicked()));
    initNewGame();
}

Quiz::~Quiz()
{
    delete ui;
}

QRadioButton *Quiz::getRadioButton(int i){
    if (i == 1) return ui->radioButton;
    if (i == 2) return ui->radioButton_2;
    if (i == 3) return ui->radioButton_3;
    if (i == 4) return ui->radioButton_4;
}

void Quiz::fillNumberQuestions(){
    QString str;
    utils::read_from_file(":/static/quiz_questions.json", str, false);
    questionsJson = utils::json_loads(str);
    int questionsCount = questionsJson["Questions"].toArray().size();
    int questionsNumbers[questionsCount];
    for (int i=0; i<questionsCount ; i++ ) {
        questionsNumbers[i] = i;
    }
    std::random_shuffle(questionsNumbers,questionsNumbers+questionsCount);
    for (int i=0; i<maxQuestions ; i++ ) {
        orderQuestions[i] = questionsNumbers[i];
    }
}


void Quiz::initNewGame(){
    question = 0;
    ui->listWidget_2->clear();
    for(int i = 0; i < maxQuestions; i++){
        ui->listWidget_2->addItem(QString::number(points[i]));
    }
    ui->listWidget_2->item(0)->setBackground(QBrush(QColor(0,255,0)));
    fifty=true;
    hundred=true;
    chance=false;
    isUsedChanse=false;
    gameOver=false;
    fillNumberQuestions();
    loadQuestionByNum(question);
    ui->Fifty->setVisible(true);
    ui->Hundred->setVisible(true);
    ui->Chance->setVisible(true);

}

bool Quiz::loadQuestionByNum(int num_question){
    questionID=orderQuestions[num_question];
    QJsonObject json = questionsJson["Questions"].toArray()[questionID].toObject();
    ui->plainTextEdit->setPlainText(json["Question"].toString());
    ui->radioButton->setText(json["A"].toString());
    ui->radioButton_2->setText(json["B"].toString());
    ui->radioButton_3->setText(json["C"].toString());
    ui->radioButton_4->setText(json["D"].toString());
    rightAnswer = json["Answer"].toString();
    for(auto item : {ui->radioButton, ui->radioButton_2, ui->radioButton_3, ui->radioButton_4}){
        item->setCheckable(false);
        item->setChecked(false);
        item->setCheckable(true);
        item->setVisible(true);
    }
    return true;
}

void Quiz::on_Fifty_clicked()
{

    if (fifty) {
        if (rightAnswer == ui->radioButton->text() || rightAnswer== ui->radioButton_3->text())
        {
            ui->radioButton_2->setVisible(false);
            ui->radioButton_4->setVisible(false);
        }
        else  if (rightAnswer== ui->radioButton_2->text() || rightAnswer== ui->radioButton_4->text())
        {
            ui->radioButton->setVisible(false);
            ui->radioButton_3->setVisible(false);
        }
    }
    fifty = false;
    ui->Fifty->setVisible(false);
}

void Quiz::on_Chance_clicked()
{
    chance = isUsedChanse = true;
    ui->Chance->setVisible(false);
}

void Quiz::on_Hundred_clicked()
{
    if (hundred) {
        for(auto item : {ui->radioButton, ui->radioButton_2, ui->radioButton_3, ui->radioButton_4}){
            if(item->text() == rightAnswer)
                item->setVisible(true);
            else
                item->setVisible(false);
        }
    }
    hundred = false;
    ui->Hundred->setVisible(false);
}

void Quiz::on_rb_clicked()
{
    if(gameOver)
        return;
    QRadioButton *rb = qobject_cast<QRadioButton*>(QObject::sender());
    if(!chance && rightAnswer != rb->text()){
        rb->setStyleSheet("color: red");
        gameOver = true;
        return;

    }
    else if (chance && rightAnswer != rb->text()){
        chance=false;
        rb->setVisible(false);
    }
    else{
        ui->listWidget_2->item(question)->setBackground(QBrush(QColor(255,255,255)));
        chance=false;
        rb->setStyleSheet("color: green");
        if(question == maxQuestions-1){
            QMessageBox mb;
            mb.setText("Вы выиграли");
            mb.exec();
            close();
        }
        else{
            //Timer1->Enabled=true;
            question++;
            rb->setStyleSheet("color: black");
            ui->listWidget_2->item(question)->setBackground(QBrush(QColor(0,255,0)));
            loadQuestionByNum(question);
        }
    }
}
