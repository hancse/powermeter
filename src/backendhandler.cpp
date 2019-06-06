#include "backendhandler.h"

#include <QDebug>

BackendHandler::BackendHandler(QWidget *parent)
{
    msg2db["pi"] = 3.14;
    //msg2db.parse();
    socket = new QSslSocket(this);
    qDebug() << socket->sslLibraryBuildVersionString();

    sslConfig.setProtocol(QSsl::TlsV1_0);

}

BackendHandler::~BackendHandler()
{

}

QByteArray BackendHandler::test_serialize()
{
    json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    json msg = "{ \"phaseVoltageL1\": 230.0, "
                 "\"phaseVoltageL2\": 231.0, "
                 "\"phaseVoltageL3\": 232.0, "
                 "\"currentL1\": 300.0, "
                 "\"currentL2\": 300.0, "
                 "\"currentL3\": 300.0, "
                 "\"power\": 300.0, "
                 "\"energy\": 300.0, "
                 "\"frequency\": 300.0, "
                 "\"timestamp\": 300.0 }"_json;

    double phaseVoltageL1 = 230.0;
    double phaseVoltageL2 = 230.0;
    double phaseVoltageL3 = 230.0;
    double currentL1 = 230.0;
    double currentL2 = 230.0;
    double currentL3 = 230.0;
    double power = 230.0;
    double energy = 230.0;
    double frequency = 230.0;
    qint64 timestamp = 1559821982;

    QString strmsg = "{ ";
    strmsg.append( QString("\"phaseVoltageL1\": %1").arg(phaseVoltageL1, 0, 'f', 1) );
    strmsg.append( QString("\"phaseVoltageL2\": %1").arg(phaseVoltageL2, 0, 'f', 1) );
    strmsg.append( QString("\"phaseVoltageL3\": %1").arg(phaseVoltageL3, 0, 'f', 1) );
    strmsg.append( QString("\"currentL1\": %1").arg(currentL1, 0, 'f', 1) );
    strmsg.append( QString("\"currentL2\": %1").arg(currentL2, 0, 'f', 1) );
    strmsg.append( QString("\"currentL3\": %1").arg(currentL3, 0, 'f', 1) );
    strmsg.append( QString("\"power\": %1").arg(power, 0, 'f', 1) );
    strmsg.append( QString("\"energy\": %1").arg(energy, 0, 'f', 1) );
    strmsg.append( QString("\"frequency\": %1").arg(frequency, 0, 'f', 1) );
    strmsg.append( QString("\"timestamp\": %1").arg(timestamp) );
    strmsg.append( " }_json");

    QByteArray bamsg = QByteArray();
    bamsg.append(strmsg);
    std::string jdmp  = j.dump();
    qDebug() << jdmp.data() << endl
             << msg.dump().data() << endl
             << bamsg;
    return bamsg;

}

//https://gist.github.com/rla/3163550
void BackendHandler::postRequest(QByteArray &postData)
{
    QUrl url = QUrl("https://hannl-sustainablecharching-be-app.azurewebsites.net");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setSslConfiguration(sslConfig);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    connect(manager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(onFinish(QNetworkReply*)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),
            manager,SLOT(deleteLater()));

    //QByteArray data = QtJson::Json::serialize(collectSyncData());
    // FIXME for debug
    //qDebug() << "Sync" << QString::fromUtf8(postData.data(), postData.size());

    manager->post(request, postData);
}

void BackendHandler::onFinish(QNetworkReply *rep)
{
    QByteArray replyba = rep->readAll();
 qDebug() << "finished" << replyba;
}


