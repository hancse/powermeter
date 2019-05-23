#ifndef PERSONCONTAINER_H
#define PERSONCONTAINER_H

#include <QObject>
#include <QJsonObject>
#include <QList>
#include "jsonserializable.h"
#include "person.h"

class PersonContainer:public JsonSerializable
{
public:
   //[...]
   //void add(Person &p);
    void write(QJsonObject &jsonObj) const;
    void read(const QJsonObject &jsonObj);

   QList<Person> getPersons() const;

private:
   QList<Person> persons;

};


#endif // PERSONCONTAINER_H
