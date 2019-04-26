/* ==============================================================
//  https://github.com/thomas92911
//  thomas92911@gmail.com
//  BTC: 1HDTGpzYCK4BBvA3ik8gX7M1f3oDkX8Aax
//
//  care not the gains but the progress
//
//  MIT License
//
//  Copyright (c) 2019 thomas92911
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// ==============================================================*/
#include "qjsonrpcclient_private.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QTimer>


QJsonRpcClient_private::QJsonRpcClient_private(QString addr, int port, bool tls, QString subMethod)
    : QObject ()
{
    _addr = addr;
    _port = port;
    _tls  = tls;

    QString strUrl;
    if (tls) {
        strUrl = "https://";
    }
    else {
        strUrl = "http://";
    }
    strUrl += addr + ":" + QString::number(port);
    if (!subMethod.isEmpty()) {
        strUrl += "/" + subMethod;
    }
    _url.setUrl(strUrl);

    connect(&_network, &QNetworkAccessManager::sslErrors, this, &QJsonRpcClient_private::sslErrors);
}

// build rpc request
QJsonObject QJsonRpcClient_private::BuildRpcRequest(QString method, QJsonObject data, uint requestID)
{
    QJsonObject req;
    req["method"] = method;
    req["id"] = static_cast<qint64>(requestID);

    QJsonArray array;
    array.append(data);

    req["params"] = array;

    return req;
}

// rpcCall
uint QJsonRpcClient_private::rpcCall(QString method, QJsonObject postData)
{
    uint rid = NewRequestID();
    QJsonObject rpcReq = BuildRpcRequest(method, postData, rid);
    QJsonDocument doc(rpcReq);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    qDebug() << "post data";
    qDebug() << data;

    QNetworkRequest req(_url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = _network.post(req, data);
    SetReplyUserdata(reply, rid);
    connect(reply, &QNetworkReply::finished, this, &QJsonRpcClient_private::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &QJsonRpcClient_private::httpReadyRead);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    return rid;
}

// set replay userdata
void QJsonRpcClient_private::SetReplyUserdata(QNetworkReply *reply, uint requestID)
{
    QString *pID = new QString(requestID);
    QTimer *timer = new QTimer();
    timer->setInterval(TIMEOUT_SECOND * 1000);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), reply, SIGNAL(finished()));
    timer->start();

    reply->setUserData(REPLY_ROLE_ID, (QObjectUserData*)pID);
    reply->setUserData(REPLY_ROLE_TIMER, (QObjectUserData*)timer);
}

// release reply
void QJsonRpcClient_private::ReleaseReply(QNetworkReply *reply)
{
    QString *pID = (QString*)reply->userData(REPLY_ROLE_ID);
    QTimer *timer = (QTimer*)reply->userData(REPLY_ROLE_TIMER);
    reply->setUserData(REPLY_ROLE_ID, nullptr);
    reply->setUserData(REPLY_ROLE_TIMER, nullptr);

    delete pID;
    delete timer;

    reply->deleteLater();
}

// new request ID
uint QJsonRpcClient_private::NewRequestID()
{
    uint rid;
    _mutex.lock();
    rid = _replyID++;
    _mutex.unlock();
    return rid;
}

void QJsonRpcClient_private::error(QNetworkReply::NetworkError)
{
    qDebug() << "reply error";
}

void QJsonRpcClient_private::httpFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    ReleaseReply(reply);

    QByteArray result = reply->readAll();
    qDebug() << result;

    qDebug() << "reply finished";
}

void QJsonRpcClient_private::httpReadyRead()
{
    qDebug() << "reply ready read";

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QString *pID = (QString*)reply->userData(REPLY_ROLE_ID);
    if (pID == nullptr) {
        qDebug() << "null id data";
        return;
    }
    uint rid = pID->toUInt();

    QByteArray result = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(result);
    QJsonObject json = doc.object();
    emit rpcReply(rid, json);

    qDebug() << result;
}

void QJsonRpcClient_private::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    reply->ignoreSslErrors();
}

