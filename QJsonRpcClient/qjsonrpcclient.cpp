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
#include "qjsonrpcclient.h"
#include "qjsonrpcclient_private.h"


QJsonRpcClient::QJsonRpcClient(QString addr, int port, bool tls) : QObject ()
{
    p = new QJsonRpcClient_private(addr, port, tls);
    connect(p, SIGNAL(rpcReply(uint,QJsonObject)), this, SIGNAL(rpcReply(uint,QJsonObject)));
}

// rpc call
uint QJsonRpcClient::rpcCall(QString method, QJsonObject postData)
{
    return p->rpcCall(method, postData);
}
