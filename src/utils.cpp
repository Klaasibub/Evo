#include "utils.h"


QString utils::base64_encode(QString src)
{
    return src.toUtf8().toBase64();
}

QString utils::base64_decode(QString src)
{
    return QByteArray::fromBase64(src.toUtf8());
}

int utils::write_to_file(QString filename, QString src, bool encoded)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 1;
    QTextStream out(&file);
    out << (encoded ? utils::base64_encode(src) : src);
    if(file.error() != QFile::NoError)
        return 2;
    file.close();
    return 0;
}

int utils::read_from_file(QString filename, QString &src, bool encoded)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QTextStream in(&file);
    QString out = "";
    out = file.readAll();
    if(file.error() != QFile::NoError)
        return 2;
    src = (encoded ? utils::base64_decode(out) : out);
    file.close();
    return 0;
}

QString utils::json_dumps(QJsonObject json)
{
    QJsonDocument doc(json);
    return QString(doc.toJson(QJsonDocument::Compact));
}

QJsonObject utils::json_loads(QString json_string)
{
    QJsonDocument doc = QJsonDocument::fromJson(json_string.toUtf8());
    QJsonObject json;
    if(doc.isObject()){
        json = doc.object();
    }
    return json;
}

bool utils::json_validation(QJsonValue original, QJsonValue schema){
    if(original.type() != schema.type())
        return false;
    else if(original.isObject()){
        QJsonObject json = original.toObject(),
                    json_schema = schema.toObject();

        for(auto key: json.keys()){
            if(json_schema.contains(key)){
               if(json_validation(json[key], json_schema[key]) == false)
                   return false;
            }
            else
                return false;
        }
        return true;
    }
    else if(original.isArray()){
        QJsonArray arr = original.toArray(),
                   arr_schema = schema.toArray();

        for(auto val: arr)
            if(json_validation(val, arr_schema[0]) == false)
                return false;
        return true;
    }
    return original.type() == schema.type();
}
