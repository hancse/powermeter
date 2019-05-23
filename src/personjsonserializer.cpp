#include "personjsonserializer.h"

#include <QJsonObject>
#include <QJsonDocument>

QString PersonJsonSerializer::serialize(const JsonSerializable &obj)
{
    QJsonObject jsonObj;
    obj.write(jsonObj);
    QJsonDocument doc(jsonObj);
    QString result(doc.toJson(QJsonDocument::Indented));
    return result;
}

void PersonJsonSerializer::parse(const QString &json, JsonSerializable &jsonSerializable)
{
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

    // The document wrap a jsonObject
    QJsonObject jsonObj = document.object();

    jsonSerializable.read(jsonObj);

}
