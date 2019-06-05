#include "backendhandler.h"

#include <QDebug>

BackendHandler::BackendHandler(QWidget *parent)
{
    msg2db["pi"] = 3.14;
}

BackendHandler::~BackendHandler()
{

}

void BackendHandler::postRequest(QByteArray & postData)
{
    QUrl url = QUrl("https://hannl-sustainablecharching-be-app.azurewebsites.net");

    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);

    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    QHttpMultiPart http;

    QHttpPart receiptPart;
    receiptPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    receiptPart.setBody(postData);

    http.append(receiptPart);

    mgr->post(QNetworkRequest(url), &http);
}

void BackendHandler::onFinish(QNetworkReply *rep)
{
 qDebug() << "finished"; // << rep->
}
