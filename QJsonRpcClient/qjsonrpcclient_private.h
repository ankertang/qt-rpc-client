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
#ifndef QJSONRPCCLIENT_PRIVATE_H
#define QJSONRPCCLIENT_PRIVATE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QSslError>
#include <QUrl>
#include <QMutex>


class QJsonRpcClient;

#define TIMEOUT_SECOND   5

#define REPLY_ROLE_ID    (Qt::UserRole + 1)
#define REPLY_ROLE_TIMER (Qt::UserRole + 2)


class QJsonRpcClient_private : public QObject
{
    Q_OBJECT
    friend class QJsonRpcClient;
private:
    QJsonRpcClient_private(QString addr, int port, bool tls, QString subMethod = "");

    // rpcCall
    uint rpcCall(QString method, QJsonObject postData);

    // build rpc request
    QJsonObject BuildRpcRequest(QString method, QJsonObject data, uint requestID);

    // set replay userdata
    void SetReplyUserdata(QNetworkReply *reply, uint requestID);

    // release reply
    void ReleaseReply(QNetworkReply *reply);

    // new request ID
    uint NewRequestID();

signals:
    void rpcReply(uint id, QJsonObject reply);

public slots:
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
    void httpFinished();
    void httpReadyRead();
    void error(QNetworkReply::NetworkError);

private:
    QNetworkAccessManager _network;
    QString _addr;
    int _port;
    bool _tls;
    QUrl _url;
    uint _replyID{0};
    QMutex _mutex;
};

#endif // QJSONRPCCLIENT_PRIVATE_H
