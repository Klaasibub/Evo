#ifndef JSON_SCHEMAS_H
#define JSON_SCHEMAS_H

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

/*!
 * \brief Модуль json шаблонов.
 *
 * Этот модуль содержит шаблоны json,
 * для проверки корректности json объектов.
 */
namespace json_schema {
     ///Набор конечных состояний шаблона.
    QJsonValue  Bool = true,
                Double = 0.0,
                String = "0";
    /// Шаблон для вопросов игры Quiz.
    QJsonObject quiz_questions
    {
        {
            "Questions", QJsonArray{
                QJsonObject{
                    {"Question", String},
                    {"A", String},
                    {"B", String},
                    {"C", String},
                    {"D", String},
                    {"Answer", String}
                }
            }
        }
    };
}

#endif // JSON_SCHEMAS_H
