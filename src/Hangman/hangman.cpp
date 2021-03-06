#include "hangman.h"
#include "ui_hangman.h"
#include "utils.h"
#include <random>
#include <QMessageBox>
#include <ctime>
#include <algorithm>
#include <QDir>
#include <QInputDialog>
#include <QLineEdit>
#include <QTextCodec>
#include <QFileInfo>

const QString Hangman::recordsPath = "static/records_hangman.csv";
const QString Hangman::aboutPath = ":/Hangman/about.txt";
const QString Hangman::previousGame = "save/prevGame.txt";

Hangman::Hangman(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Hangman)
{
    ui->setupUi(this);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setStyle(":/Hangman/Style_Hangman.css");
    for(auto bt: {ui->buttonA, ui->buttonB, ui->buttonV, ui->buttonG, ui->buttonD, ui->buttonYE,
        ui->buttonYE1, ui->buttonZH, ui->buttonZ, ui->buttonI, ui->buttonYI, ui->buttonK, ui->buttonL,
        ui->buttonM, ui->buttonN, ui->buttonO, ui->buttonP, ui->buttonR, ui->buttonS, ui->buttonT,
        ui->buttonU, ui->buttonF, ui->buttonKH, ui->buttonTC, ui->buttonCH, ui->buttonSH, ui->buttonSHCH,
        ui->buttonTv, ui->buttonY, ui->buttonMya, ui->buttonE, ui->buttonYU, ui->ButtonYA})
        connect(bt, SIGNAL(clicked()), this, SLOT(onLetterClicked()));
    setStartData(":/Hangman/words.txt");
    ui->label_score->setText("Текущий результат: " + QString::number(score));
    startPol();

    QFileInfo check_file(previousGame);
       if (check_file.exists() && check_file.isFile()) {
            loadGame();
        }
        else {
            saveGame();
        }

}

Hangman::~Hangman()
{
    delete ui;
}

void Hangman::onLetterClicked()
{
    bool point = false;
    QPushButton* bt = qobject_cast<QPushButton*>(sender());
    QString  w = words[current_word];
    QString let = bt -> text();
    bt->setEnabled(false);
    int kol = 0;
    for (int i = 0; i < w.length(); i++){
        if (let[0] ==  word_on_screen[i]){
            return;
        }
        if (w[i] == let[0]){
            word_on_screen[i] = let[0];
            point = true;
            kol+=1;
        }
    }
    ui->label_word->setText(word_on_screen);
    if (point){
        score += kol*score_m;
        score_m += 1;
        saveGame();
    }
    else{
        score_m = 1;
        mistakes+=1;
        showPic(mistakes);
        if (mistakes == 9){
             check_records();
             QFile(QDir::currentPath() + "/" + Hangman::previousGame).remove();
             QMessageBox::information(this, "вы проиграли", "Ваш результат: "+ QString::number(score));
             this->close();
        }
    }
    ui->label_score->setText("Текущий результат: " + QString::number(score));
    if (word_on_screen.indexOf("?", 0) == -1){
        current_word+=1;
        if (current_word < words.length()){
            startPol();
        }
        else{
            check_records();
            QFile(QDir::currentPath() + "/" + Hangman::previousGame).remove();
            QMessageBox::information(this, "Игра окончена", "Ваш результат: "+ QString::number(score));
            this->close();
        }
    }
}

void Hangman:: showPic(int p)
{
    QString css;
    css = "background-image: url(:/Hangman/" + QString::number(p) + ".png);";
    ui->label_hangman->setStyleSheet(css);
}

void Hangman:: startPol()
{
    word_on_screen = words[current_word];
    word_on_screen.replace(QRegExp("."), "?");
    ui->label_word->setText(word_on_screen);
    ui -> label_hangman -> setStyleSheet("");
    for (int i = 0; i < ui->gridLayout->count(); ++i){
          QWidget* widget = ui->gridLayout->itemAt(i)->widget();
          if (widget != NULL)
          {
            QPushButton* item = qobject_cast<QPushButton*>(widget);
            item->setEnabled(true);
          }
        }
}

void Hangman::on_pushButton_show_clicked()
{
    showLetter();
    ui->label_score->setText("Текущий результат: " + QString::number(score));
    ui ->pushButton_show->setEnabled(false);
}

void Hangman::on_pushButton_second_clicked()
{
   mistakes = -1;
   ui -> label_hangman -> setStyleSheet("");
   ui ->pushButton_second->setEnabled(false);
}

void Hangman::check_records()
{
    QVector<QPair<QString, int>> records;
    QString data;
    QString record_path = QDir::currentPath() + "/" + Hangman::recordsPath;
    utils::read_from_file(record_path, data, false);
    QStringList rowData, rowsData = data.split("\n");
    for (int i = 1; i < rowsData.size(); i++){
        rowData = rowsData.at(i).split(";");
        if (rowData.size()>1){
            records.push_back(QPair<QString, int>(rowData[0], rowData[1].toInt()));
        }
    }
    std::sort(records.begin(),records.end(),this->comp);
    if (records.size()<20 || records[19].second<score){
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
                records.append(QPair<QString, int>(str,score));
            }
            else{
                 records[19].first = str;
                 records[19].second = score;
            }
        }
    }
    std::sort(records.begin(),records.end(),this->comp);
    data = "Nickname;Score\n";
    for(auto i: records){
        data += i.first + ";" + QString::number(i.second) + "\n";
    }
    utils::write_to_file(record_path, data, false);
}

bool Hangman::comp (QPair <QString, int > a, QPair <QString, int > b)
{
  return a.second > b.second;
}

void Hangman::setStartData(QString s)
{
    current_word = 0;
    mistakes = -1;
    score_m = 1;
    score = 0;
    QString out;
    utils::read_from_file(s, out, false);
    words = out.split("\n");
    if(words.size() && words[words.size()-1] == "")
        words.pop_back();
    srand(time(NULL));
    std::random_shuffle(words.begin(), words.end());
}

void Hangman::setStyle(QString s)
{
    QString style;
    utils::read_from_file(s, style, false);
    setStyleSheet(style);
}

void Hangman::showLetter()
{
    int score1 = score;
    QVector<int> vec;
    for (int i = 0; i<words[current_word].length(); i++){
        if (word_on_screen[i] == "?"){
            vec.push_back(i);
        }
    }
    if (vec.size() == 0){
        return;
    }
    std::random_shuffle(vec.begin(), vec.end());
    int n = vec[0];

    for (int i = 0; i < ui->gridLayout->count(); ++i){
            QWidget* widget = ui->gridLayout->itemAt(i)->widget();
            if (widget != NULL)
            {
                QPushButton* item = qobject_cast<QPushButton*>(widget);
                if(words[current_word][n] == item->text()){
                    item->click();
                }
            }
    }
    score = score1;
    score_m = 1;
}

void Hangman::saveGame(){
    QString data;
    QString previousGame_path = QDir::currentPath() + "/" + Hangman::previousGame;
    data = QString::number(current_word) + "\n";
    data += word_on_screen + "\n";
    data += QString::number(score) + "\n";
    data += QString::number(score_m) + "\n";
    data += QString::number(mistakes) + "\n";
    for (int i = 0; i<words.length();i++){
         data += words[i] + "\n";
    }
    utils::write_to_file(previousGame_path, data, false);
}

void Hangman::loadGame(){
    QString data;
    QString previousGame_path = QDir::currentPath() + "/" + Hangman::previousGame;
    int i = 0;
    QFile inputFile(previousGame_path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
           switch(i){
             case 0:
                current_word = in.readLine().toInt();
                break;
             case 1:
                word_on_screen = in.readLine();
                break;
             case 2:
                score = in.readLine().toInt();
                break;
             case 3:
                score_m = in.readLine().toInt();
                break;
             case 4:
                mistakes = in.readLine().toInt();
                break;
             default:
                words[i-5] = in.readLine();
                break;
           }
          i++;
       }
       inputFile.close();
    }
    else{
        QMessageBox::information(this, "Ошибка загрузки", "Невозможно загрузить предыдущее сохранение");
        this->close();
    }
    ui->label_word->setText(word_on_screen);
    ui->label_score->setText("Текущий результат: " + QString::number(score));
    showPic(mistakes);
}
