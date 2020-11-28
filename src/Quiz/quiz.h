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
/*!
 * \brief модуль игры Викторина
 *
 * Этот модуль содержит основную логику игры "Викторина".
 */

class Quiz : public QDialog
{
    Q_OBJECT

public:
    /// путь до файла с рекордами и до файла содержащего информацию об игре
    static const QString recordsPath, aboutPath;
    /// переменная содержащая максимальное количество вопросов в файле
    static const int maxQuestions = 27;
    /// Конструктор класса Викторина.
    explicit Quiz(QWidget *parent = nullptr);
    /// Деструктор класса Викторина.
    ~Quiz();
    /*!
     * \brief Заполнение списка вопросов из файла
     *
     */
    void fillNumberQuestions();
    /*!
     * \brief Загрузка вопросов на игровое поле
     * \param num_question
     * \return
     *
     */
    bool loadQuestionByNum(int num_question);
    /*!
     * \brief инициализация Новой игры с обновлением всех параметров
     *
     */
    void initNewGame();
    /*!
     * \brief Функция сравнения двух элементов.
     * \param a - первый элемент стравнения.
     * \param b - второй элемент стравнения.
     * \return результат сравнения двух элементов.
     */
    static bool comp(QPair <QString, int > a, QPair <QString, int > b);
    /*!
     * \brief Загрузка стилей.
     *
     * Считывание QSS-стилей и применение их к форме.
     */
    void loadStyle();


private slots:
    /*!
     * \brief использование подсказки "50:50"
     *
     * удаление 2-х неправильных ответов с игрового поля
     */
    void on_Fifty_clicked();
    /*!
     * \brief использование подсказки "Право на ошибку"
     *
     * удаление ответа с игрового поля, если он неправильный и продолжение игры
     */
    void on_Chance_clicked();
    /*!
     * \brief использование подсказки "Правильный ответ"
     *
     * удаление с игровокого поля всех ответов кроме правильного
     */
    void on_Hundred_clicked();
    /*!
     * \brief проверка нажатого RadioButton на правильность
     *
     */
    void onRbClicked();


private:
    Ui::Quiz *ui;
    int orderQuestions[maxQuestions];/// массив с номерами порядка вопросов
    bool fifty, /// проверка использования подсказки "50:50"
        hundred, /// проверка использования подсказки "Правильный ответ"
        chance, /// проверка использования подсказки "Право на ошибку"
        gameOver;/// проверка окончания игры
    int question, /// номер вопроса
        questionID; /// храненилище для номера вопроса из массива orderQuestions[]
    QJsonObject questionsJson; /// список вопросов
    QString rightAnswer; /// правильный ответ
    /*!
     * \brief Вывод таблицы рекордов
     *
     */
    void check_records();


};

#endif // QUIZ_H
