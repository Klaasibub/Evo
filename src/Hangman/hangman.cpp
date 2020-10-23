#include "hangman.h"
#include "ui_hangman.h"
#include "utils.h"
#include <random>
#include <QMessageBox>
#include <ctime>

const QString recordsPath = "static/records_hangman.txt";
const QString aboutPath = "static/records_hangman.txt";

QStringList words;
int current_word, mistakes, score, score_m;
QString word_on_screen;
//QPushButton* bt1;

Hangman::Hangman(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Hangman)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QString out;
    utils::read_from_file(":/static/Hangman/words.txt", out, false);
    words = out.split("\n");

    words.pop_back();

    srand(time(NULL));
    std::random_shuffle(words.begin(), words.end());

   // bt1 = new QPushButton;

    for(auto bt: {ui->button000, ui->button224, ui->button225, ui->button226, ui->button227, ui->button228,
        ui->button229, ui->button230, ui->button231, ui->button232, ui->button233, ui->button234, ui->button235,
        ui->button236, ui->button237, ui->button238, ui->button239, ui->button240, ui->button241, ui->button242,
        ui->button244, ui->button245, ui->button246, ui->button247, ui->button248, ui->button249, ui->button250,
        ui->button251, ui->button252, ui->button253, ui->button254, ui->button255/*, bt1*/})
        connect(bt, SIGNAL(clicked()), this, SLOT(on_letter_clicked()));
    score = 0;
    current_word = 0;
    score_m = 1;

    startPol();

    ui->label_score->setText("Текущий результат: " + QString::number(score));
    //кнопка для подсказки буквы
    ui ->pushButton_show->setVisible(false);
}

Hangman::~Hangman()
{
    delete ui;
}

void Hangman::on_letter_clicked()
{
    bool point = false;
    QPushButton* bt = qobject_cast<QPushButton*>(sender());
    QString  w = words[current_word];
    QString let = bt -> text();

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
    }

    else{
        score_m = 1;
        mistakes+=1;
        showPic(mistakes);
        if (mistakes == 9){
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
            QMessageBox::information(this, "Игра окончена", "Ваш результат: "+ QString::number(score));
            this->close();
        }
    }
}

void Hangman:: showPic(int p){
    QString css;
    css = "background-image: url(:/static/Hangman/" + QString::number(p) + ".png);";
    ui->label_hangman->setStyleSheet(css);
}

void Hangman:: startPol(){
    word_on_screen = words[current_word];
    word_on_screen.replace(QRegExp("."), "?");
    ui->label_word->setText(word_on_screen);

    mistakes = -1;

    ui -> label_hangman -> setStyleSheet("");
}
/*
void Hangman::on_pushButton_show_clicked()
{
    int score1 = score, scorem = score_m;
    QVector<int> vec;

    for (int i = 0; i<words[current_word].length(); i++){
        if (word_on_screen == '?'){
            vec.push_back(i);
        }
    }
    std::random_shuffle(vec.begin(), vec.end());

    bt1->setText( QString(words[current_word][vec[0]]));
    bt1->click();
    score = score1;
    score_m = scorem;
}
*/
void Hangman::on_pushButton_second_clicked()
{
   mistakes = -1;
   ui -> label_hangman -> setStyleSheet("");
   ui ->pushButton_second->setEnabled(false);
}
