#ifndef PERSONJSONSERIALIZER_H
#define PERSONJSONSERIALIZER_H

#include "jsonserializable.h"

//To make it easy to serialize and deserialize
//JsonSerializable objects I made
//the class PersonJsonSerializer
//with two static methods serialize and parse:

class PersonJsonSerializer
{
public:
    static QString serialize(const JsonSerializable &obj);
    static void parse(const QString &json, JsonSerializable &jsonSerializable);

private:
    PersonJsonSerializer();

};

#endif // PERSONJSONSERIALIZER_H
