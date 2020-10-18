#ifndef JSON_SCHEMAS_H
#define JSON_SCHEMAS_H

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
namespace json_schema {
    QJsonValue  Bool = true,
                Double = 0.0,
                String = "0";

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
