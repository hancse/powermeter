#ifndef BACKENDHANDLER_H
#define BACKENDHANDLER_H

#include <QObject>

#include "thirdparty/json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

class BackendHandler : public QObject
{
    Q_OBJECT
public:
    BackendHandler();
    ~BackendHandler();

signals:

public slots:

private:
    json msg2db;
};

#endif // BACKENDHANDLER_H
