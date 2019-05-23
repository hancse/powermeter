#include "personcontainer.h"

#include <QJsonArray>

void PersonContainer::read(const QJsonObject &jsonObj)
{
    // json encapsulates the QJsonArray
    QJsonArray jsonArray = jsonObj["persons"].toArray();
    foreach(QJsonValue jsonPerson, jsonArray)
    {
        Person p;
        // the QJsonValue encapsulates the person QJsonObject
        p.read(jsonPerson.toObject());
        this->persons.append(p);
    }
}

void PersonContainer::write(QJsonObject &jsonObj) const
{
    QJsonArray jsonArray;
    foreach (Person p, this->persons)
    {
        QJsonObject jsonPerson;
        p.write(jsonPerson);
        jsonArray.append(jsonPerson);
    }

    jsonObj["persons"] = jsonArray;
}
