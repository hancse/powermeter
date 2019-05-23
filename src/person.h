#ifndef PERSON_H
#define PERSON_H

#include <QJsonObject>
#include "jsonserializable.h"

//My JSON serializable classes implement that interface.
//Let’s begin with a simple Person class.

class Person: public JsonSerializable
{
public:
    void read(const QJsonObject &jsonObj);
    void write(QJsonObject &jsonObj) const;

    void setId(const qint64 &value);
    void setName(const QString &value);
    void setSurname(const QString &value);
    void setEmail(const QString &value);

    qint64 getId() const;
    QString getName() const;
    QString getSurname() const;
    QString getEmail() const;

private:
    qint64 id;
    QString name;
    QString surname;
    QString email;
};

#endif // PERSON_H
