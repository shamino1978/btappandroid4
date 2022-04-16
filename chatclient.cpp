/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chatclient.h"

#include <QtCore/qmetaobject.h>

#include "Global_Var.h"
#include <QFuture>
#include <QtConcurrent>

ChatClient::ChatClient(QObject *parent)
    :   QObject(parent)
{
}

ChatClient::~ChatClient()
{
    stopClient();
}

void ChatClient::read_socket_other_thread(){
connect(socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
}

//! [startClient]
void ChatClient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;


    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

   // connect(socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);

   // QFuture<void> future = QtConcurrent::run(this, &ChatClient::read_socket_other_thread);
    //QFuture<void> future = QtConcurrent::run(this, &ChatClient::keep_reading);




    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&ChatClient::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatClient::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);

}
//! [startClient]

//! [stopClient]
void ChatClient::stopClient()
{
    delete socket;
    socket = nullptr;

}
//! [stopClient]



void ChatClient::keep_reading()
{
    qDebug() << "chat server something to read";
    while(1){
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (socket){

    qint64 total_read=0;
    int data_index=0;
    int counter=0;
    int total_tries=0;
    while(counter<5){
    while(socket->bytesAvailable()){
        qint64 read_bytes= socket->read((&data_array_bluetooth[data_index]), 50000);
        data_index+=read_bytes;
        total_read+=read_bytes;
    }counter++;
    total_tries++;
    if(total_tries>10000)
        break;
    if(data_array_bluetooth[data_index-1]!=123){
        counter=0;
    }
    //clock_nanosleep(1000);
    }
    if(total_read>0){
    emit messageReceived2((int)total_read);
    }
}
    }
    //    emit messageReceived(socket->peerName(),
      //                       QString::fromUtf8(line.constData(), line.length()));

}


//! [readSocket]
void ChatClient::readSocket()
{
    qDebug() << "chat server something to read";
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;
    qint64 total_read=0;
    int data_index=0;
    int counter=0;
    int total_tries=0;
    while(counter<5){
    while(socket->bytesAvailable()){
        qint64 read_bytes= socket->read((&data_array_bluetooth[data_index]), 50000);
        data_index+=read_bytes;
        total_read+=read_bytes;
    }counter++;
    total_tries++;
    if(total_tries>10000)
        break;
    if(data_array_bluetooth[data_index-1]!=123){
        counter=0;
    }
    //clock_nanosleep(1000);
    }
    emit messageReceived2((int)total_read);

    //    emit messageReceived(socket->peerName(),
      //                       QString::fromUtf8(line.constData(), line.length()));

}
//! [readSocket]

//! [sendMessage]
void ChatClient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}
//! [sendMessage]

void ChatClient::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
}

//! [connected]
void ChatClient::connected()
{
    emit connected(socket->peerName());
}
//! [connected]
