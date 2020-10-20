#include "quiz.h"
#include "ui_quiz.h"
#include <random>
#include <QTimer>

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
void Quiz::OrderQuestion()
{
        std::random_shuffle(OrderQuestions,OrderQuestions+14);
}

void Quiz::initNewGame(){
    question = 0;
    ui->listWidget_2->clear();
    for(int i = 0; i < maxQuestions; i++){
        ui->listWidget_2->addItem(QString::number(points[i]));
    }
    ui->listWidget_2->item(0)->setBackground(QBrush(QColor(0,255,0)));
    fifty=hundred=true;
    chance=isUsedChanse=false;
    OrderQuestion();
    LoadQuestionByNum(question);
    ui->Fifty->setVisible(true);
    ui->Hundred->setVisible(true);
    ui->Chance->setVisible(true);
}
bool Quiz::LoadQuestionByNum(int num_question){
    ui->listWidget_2->addItem(QString::number(num_question));
    num_question=OrderQuestions[num_question-1];
    ui->plainTextEdit->setPlainText("Текст");
    ui->radioButton->setText("A"+QString::number(question));
    ui->radioButton_2->setText("B"+QString::number(question));
    ui->radioButton_3->setText("C"+QString::number(question));
    ui->radioButton_4->setText("D"+QString::number(question));
    rightAnswer =1;
    return true;
}

void Quiz::on_Fifty_clicked()
{
    if (fifty) {
        if (rightAnswer==1 || rightAnswer==3)
        {
            ui->radioButton_2->setVisible(false);
            ui->radioButton_4->setVisible(false);
        }
        else  if (rightAnswer==2 || rightAnswer==4)
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
        for(int i = 1; i <= 4; i++)
            if(rightAnswer!=i){
                getRadioButton(i)->setVisible(false);
            }
    }
    hundred = false;
    ui->Hundred->setVisible(false);
}

void Quiz::on_rb_clicked()
{
    int checkedRadioBtn = 0;
    for (int i = 1; i <= 4; i++)
        if(getRadioButton(i)->isChecked()){
            checkedRadioBtn = i;
            break;
        }
    if (checkedRadioBtn == 0)
        return;
    if(!chance && rightAnswer != checkedRadioBtn){
        getRadioButton(checkedRadioBtn)->setStyleSheet("color: red");
        //Timer2->Enabled=true;

    }
    else if (chance && rightAnswer != checkedRadioBtn){
        chance=false;
        getRadioButton(checkedRadioBtn)->setVisible(false);
    }
    else{
        ui->listWidget_2->item(question)->setBackground(QBrush(QColor(255,255,255)));
        chance=false;
        getRadioButton(checkedRadioBtn)->setStyleSheet("color: green");
        if(question == maxQuestions){
            //Form3->Label1->Caption=Form1->Label26->Caption;
            //Form3->ShowModal();
        }
        else{
            //Timer1->Enabled=true;
            question++;
            ui->listWidget_2->item(question)->setBackground(QBrush(QColor(0,255,0)));
        }
    }
    getRadioButton(checkedRadioBtn)->setChecked(false);
}
