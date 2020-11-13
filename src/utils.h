#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

/*!
 * \brief Модуль утилит.
 *
 * Этот модуль содержит общие вспомогательные фунции,
 * использующиеся во всём проекте.
 */
namespace utils {

    /*!
     * \brief Кодирование строки в base64.
     *
     * Функция, которая кодирует входную строку в base64 формат.
     * \param src - Строка, которая будет закодирована в base64.
     * \return Закодированная строка.
     */
    QString base64_encode(QString src);

    /*!
     * \brief Декодирование строки из base64.
     *
     * Функция, которая декодирует входную строку из base64 формата.
     * \param src - Строка, которая будет раскодированна из base64 формата.
     * \return Раскодированная строка.
     */
    QString base64_decode(QString src);

    /*!
     * \brief Запись в файл.
     *
     * Функция, которая записывает информацию в файл.
     * \param filename - Путь к файлу, в который произведется запись.
     * \param src - Информация, которая будет записана в файл.
     * \param encoded - Параметр, отвечающий за то,
     *                  будет ли информация кодироваться в base64 или нет.
     * \return Код завершения функции.
     */
    int write_to_file(QString filename, QString src, bool encoded);

    /*!
     * \brief Считывание из файла.
     *
     * Функция, которая считывает информацию из файла.
     * \param filename - Путь к файлу, из которого нужно считать информацию.
     * \param src - Переменная, в которую запишется информация о файле.
     * \param encoded - Параметр, отвечающий за то,
     *                  будет ли информация дедироваться из base64 или нет.
     * \return Код завершения функции
     */
    int read_from_file(QString filename, QString &src, bool encoded);

    /*!
     * \brief Перевод QJsonObject в QString.
     *
     * Функция, которая сериализует QJsonObject в строку формата json.
     * \param json - объект Json, который будет сериализован.
     * \return Сериализованная строка.
     */
    QString json_dumps(QJsonObject json);

    /*!
     * \brief Перевод QString в QJsonObject.
     *
     * Функция, которая десериализует строку в QJsonObject.
     * \param json_string - Строка, которая будет десериализованна.
     * \return объект Json.
     */
    QJsonObject json_loads(QString json_string);

    /*!
     * \brief Валидатор для json объектов.
     *
     * Функция, которая проверяет правильность объекта json путем сравнения его с шаблоном.
     * \param original - json объект, который необходимо провалидировать.
     * \param schema - json объект, явлюящийся шаблоном, для валидации.
     * \return Соответствует ли json объект шаблону.
     */
    bool json_validation(QJsonValue original, QJsonValue schema);
}

#endif // UTILS_H
