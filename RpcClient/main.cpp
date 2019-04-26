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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qjsonrpcclient.h"
#include <QJsonObject>
#include <QDebug>
#include <QThread>


QJsonRpcClient *__rpc = nullptr;

QObject *jsonRpcProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    qDebug() << "jsonRpcProvider";
    return __rpc;
}


int main(int argc, char *argv[])
{
    __rpc = new QJsonRpcClient("127.0.0.1", 888, true);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<QJsonRpcClient>("PM.JsonRpc", 1, 0, "JsonRpc", jsonRpcProvider);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    // test call use C++
    QJsonObject data;
    data["Who"] = "c++";
    __rpc->rpcCall("CallService.Call", data);


    return app.exec();
}
