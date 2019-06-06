#ifndef BACKENDHANDLER_H
#define BACKENDHANDLER_H

#include <QObject>
#include <QtNetwork>
//#include <QtJson>
#include <QSsl>
//#include <QSslSocket>
//#include <QSslConfiguration>

#include "thirdparty/json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

class BackendHandler : public QObject
{
    Q_OBJECT
public:
    BackendHandler(QWidget *parent);
    ~BackendHandler();

    void postRequest(QString ipName, QByteArray &postData);
    int postRequest2(QString requestType, QUrl params);
    QByteArray test_serialize();
signals:

public slots:

private slots:
    void onFinish(QNetworkReply *rep);

private:
    json msg2db;
    //QSslSocket* socket;
    //QSslConfiguration sslConfig;
    //QSslSocket::sslLibraryBuildVersionString();

};

#endif // BACKENDHANDLER_H
