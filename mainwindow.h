#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "ui_mainwindow.h"

#include <QtWidgets/qdialog.h>
#include <QMainWindow>
#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>

#include <QtBluetooth/qbluetoothuuid.h>
//#include <qbluetoothdevicediscoveryagent.h>

//#include <qbluetoothsocket.h>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include "remoteselector2.h"
#include <QtCore/qobject.h>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
//#include "chatclient.h"


QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE



QT_USE_NAMESPACE

class ChatServer;
class ChatClient;

//! [declaration]
class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
     Ui_mainwindow *ui;
 };

#endif // MAINWINDOW_H
