#include "person.h"

#include <QVariantMap>

//The read method copies data from the QJsonObject
//into the Person member variables
//to do the step QJsonObject ==> MyObject

void Person::read(const QJsonObject &jsonObj)
{
    QVariantMap jsonMap = jsonObj.toVariantMap();
    this->setName(jsonMap["name"].toString());
    this->setSurname(jsonMap["surname"].toString());
    this->setEmail(jsonMap["email"].toString());
    this->setId(jsonMap["id"].toInt());
}

//The write method fills QJsonObjects
//with our Person member variables
//to do the step MyObject ==> QJsonObject

void Person::write(QJsonObject &jsonObj) const
{
    jsonObj["name"] = this->name;
    jsonObj["surname"] = this->surname;
    jsonObj["email"] = this->email;
    jsonObj["id"] = this->id;
}

void Person::setId(const qint64 &value)
{
    id = value;
}

void Person::setName(const QString &value)
{
    name = value;
}

void Person::setSurname(const QString &value)
{
    surname = value;
}

void Person::setEmail(const QString &value)
{
    email = value;
}

qint64 Person::getId() const
{
    return id;
}

QString Person::getName() const
{
    return name;
}

QString Person::getSurname() const
{
    return surname;
}

QString Person::getEmail() const
{
    return email;
}
