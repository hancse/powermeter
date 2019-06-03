#ifndef BACKENDHANDLER_H
#define BACKENDHANDLER_H

#include <QObject>
#include <QtNetwork>

#include "thirdparty/json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

class BackendHandler : public QObject
{
    Q_OBJECT
public:
    BackendHandler();
    ~BackendHandler();

    void postRequest(QByteArray &postData);
signals:

public slots:

private:
    json msg2db;
    void onFinish(QNetworkReply *rep);
};

#endif // BACKENDHANDLER_H
