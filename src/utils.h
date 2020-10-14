#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

namespace utils {
    QString base64_encode(QString src);
    QString base64_decode(QString src);

    int write_to_file(QString filename, QString src, bool encoded);
    int read_from_file(QString filename, QString &src, bool encoded);

    QString json_dumps(QJsonObject json);
    QJsonObject json_loads(QString json_string);
    bool json_validation(QJsonValue original, QJsonValue schema);
}

#endif // UTILS_H
