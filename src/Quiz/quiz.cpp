#include "quiz.h"
#include "ui_quiz.h"
#include <random>
#include <vector>
#include <utils.h>
#include <QMessageBox>
#include <algorithm>
#include <QDir>
#include <QTextCodec>
#include <QInputDialog>
#include <QLineEdit>

const QString Quiz::recordsPath = "static/records_quiz.csv";
const QString Quiz::aboutPath = "static/records_quiz.txt";

Quiz::Quiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Quiz)
{  
    ui->setupUi(this);
    loadStyle();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for(auto item : {ui->radioButton, ui->radioButton_2, ui->radioButton_3, ui->radioButton_4})
        connect(item, SIGNAL(clicked()),this,SLOT(on_rb_clicked()));
    initNewGame();


    QString style;
    //utils::read_from_file(":/static/Hangman/Style_Quiz.css", style, false);
    //setStyleSheet(style);

}

Quiz::~Quiz()
{
    delete ui;
}

void Quiz::fillNumberQuestions(){
    QString str;
    utils::read_from_file(":/static/quiz_questions.json", str, false);
    questionsJson = utils::json_loads(str);
    int questionsCount = questionsJson["Questions"].toArray().size();
    std::vector<int> questionsNumbers;
    for (int i=0; i < questionsCount ; i++ ) {
        questionsNumbers.push_back(i);
    }
    std::random_shuffle(questionsNumbers.begin(), questionsNumbers.end());
    for (int i = 0; i < maxQuestions; i++) {
        orderQuestions[i] = questionsNumbers[i];
    }
}

void Quiz::loadStyle(){
    QString css;
    css ="QDialog {border-image: url(:/static/Quiz/fon.jpg)};";
    setStyleSheet(css);
}

void Quiz::initNewGame(){
    question = 0;
    ui->listWidget_2->clear();
    for(int i = 0; i < maxQuestions; i++){
        ui->listWidget_2->addItem(QString::number(i+1));
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
        QMessageBox mb;
        mb.setWindowTitle("Проигрыш!");
        mb.setText("Ответ неверный. Игра окончена.");
        mb.exec();
        check_records();
        gameOver = true;
        close();
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
            check_records();
            close();
        }
        else{
            question++;
            rb->setStyleSheet("color: black");
            ui->listWidget_2->item(question)->setBackground(QBrush(QColor(0,255,0)));
            loadQuestionByNum(question);
        }
    }
}

void Quiz::check_records(){
    QVector<QPair<QString, int>> records;
    QString data;
    QString record_path = QDir::currentPath() + "/" + Quiz::recordsPath;
    utils::read_from_file(record_path, data, false);
    QStringList rowData, rowsData = data.split("\n");

    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        if (rowData.size()>1){
            records.push_back(QPair<QString, int>(rowData[0], rowData[1].toInt()));
        }
    }
    std::sort(records.begin(),records.end(),this->comp);

    if (records.size()<20 || records[19].second<question){
        bool bOk;
        QString str = QInputDialog::getText( 0,
                                             "Введите имя",
                                             "Ваше имя:",
                                             QLineEdit::Normal,
                                             "",
                                             &bOk
                                            );
        if (bOk) {
            if (records.size()<20){
                records.append(QPair<QString, int>(str,question));
            }
            else{
                 records[19].first = str;
                 records[19].second = question;
            }
        }
    }

    std::sort(records.begin(),records.end(),this->comp);

    data = "Nickname;Questions\n";

    for(auto i: records){
        data += i.first + ";" + QString::number(i.second) + "\n";
    }

    utils::write_to_file(record_path, data, false);
}

bool Quiz::comp (QPair <QString, int > a, QPair <QString, int > b) {
  return a.second > b.second;
}
