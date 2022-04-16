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

#ifndef CHAT_H_
#define CHAT_H_

#include "ui_chat.h"

#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>

#include <QtBluetooth/qbluetoothuuid.h>
//#include <qbluetoothdevicediscoveryagent.h>

//#include <qbluetoothsocket.h>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qbluetoothsocket.h>
//#include "remoteselector2.h"
#include <QtCore/qobject.h>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
//#include "chatclient.h"
#include <QMainWindow>
#include <QtWidgets>
#include <vector>
#include "mainwindow4.h"

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

using namespace std;

class intel1 : public QWidget
{
    Q_OBJECT

public:
    explicit intel1(QWidget *parent = 0);
 ~intel1();

    QLabel *vrm_voltage_label = nullptr;
    QLabel *vrm_loadline_label = nullptr;

    QLineEdit *vrm_voltage_lineedit = nullptr;
    QLineEdit *vrm_loadline_lineedit = nullptr;
    QPushButton *apply_vrm_button=nullptr;
    QVBoxLayout* vbox1=nullptr;
     QVBoxLayout* vbox2=nullptr;
     QHBoxLayout* hbox1=nullptr;
    QSpacerItem* spacer1=nullptr;
    QFrame *line1=nullptr;
      QVBoxLayout* vbox3=nullptr;
//
        QSpacerItem* spacer2=nullptr;
 QLabel *svid_voltage_label = nullptr;
QLabel *domain_label = nullptr;
QLineEdit *svid_voltage_lineedit = nullptr;
QComboBox* domain_menu= nullptr;
 QVBoxLayout* vbox4=nullptr;

QButtonGroup *radiogroup1 = nullptr;
QRadioButton *adaptive_radio= nullptr;
QRadioButton *manual_radio= nullptr;
QRadioButton *offset_radio= nullptr;
 QVBoxLayout* vbox5=nullptr;

 QPushButton *apply_svid_button=nullptr;
  QPushButton *maxout_button=nullptr;
   QVBoxLayout* vbox6=nullptr;

   QHBoxLayout* hbox2=nullptr;
   QSpacerItem* spacer3=nullptr;
   QFrame *line2=nullptr;
    QVBoxLayout* vbox7=nullptr;
//

    QLabel *ratio_limit_label = nullptr;
    QLabel *act_cores_label = nullptr;
    QLabel *atom_ratio_label = nullptr;
    QLabel *act_atom_label = nullptr;
    vector<vector< QLineEdit *>> ratio_limit_vec;
  QPushButton *apply_ratios_button=nullptr;
//
   QLabel *synch_allcores_label = nullptr;
   QLabel *synch_allatom_label = nullptr;
    QLabel *fixed_ring_label = nullptr;
    QPushButton *apply_core_button=nullptr;
     QPushButton *apply_ring_button=nullptr;

     QLabel *bclk_label = nullptr;
     QLabel *pcie_label = nullptr;
    QPushButton *apply_bclk_button=nullptr;
    QLineEdit *synch_allcores_lineedit = nullptr;
    QLineEdit *synch_allatom_lineedit = nullptr;
    QLineEdit *fixed_ring_lineedit = nullptr;
    QLineEdit *bclk_lineedit = nullptr;
    QLineEdit *pcie_lineedit = nullptr;

public slots:
     void   apply_vrm_button_pressed();
     void apply_svid_button_pressed();
    void maxout_button_pressed();
   void apply_ratios_button_pressed();
   void apply_core_button_pressed();
   void apply_ring_button_pressed();
  void  apply_bclk_button_pressed();

signals:
      void send_cmd(vector<char> &vec);


};




class intel2 : public QWidget
{
    Q_OBJECT

public:
    explicit intel2(QWidget *parent = 0);
      ~intel2();

    vector<QLabel *> qlabel_vec1;
    vector<QLineEdit*> core_ratio_vec;
    vector<QLineEdit*> core_volt_vec;
    vector<QCheckBox*> override_vec;
    vector<QLineEdit*> offset_vec;
    QSpacerItem* seperator=nullptr;
    QSpacerItem* spacer1=nullptr;
    QVBoxLayout* mainvbox=nullptr;
    QScrollArea* scrollArea = nullptr;
    QScrollArea* m_scrollArea= nullptr;
    QLabel* spco= nullptr;
    QLabel* ovc= nullptr;
    QLabel* ovr_mode= nullptr;
    QLabel* offsetv= nullptr;
    QPushButton* apply=nullptr;
    QPushButton* synchall=nullptr;
    QCheckBox* bycore=nullptr;
signals:

     void send_cmd(vector<char>&to_send);
public slots:
     void synch_all();
     void apply_this();
/*
    QLabel *vrm_voltage_label = nullptr;
    QLabel *vrm_loadline_label = nullptr;

    QLineEdit *vrm_voltage_lineedit = nullptr;
    QLineEdit *vrm_loadline_lineedit = nullptr;
    QPushButton *apply_vrm_button=nullptr;
    QVBoxLayout* vbox1=nullptr;
     QVBoxLayout* vbox2=nullptr;
     QHBoxLayout* hbox1=nullptr;
    QSpacerItem* spacer1=nullptr;
    QFrame *line1=nullptr;
      QVBoxLayout* vbox3=nullptr;
//
        QSpacerItem* spacer2=nullptr;
 QLabel *svid_voltage_label = nullptr;
QLabel *domain_label = nullptr;
QLineEdit *svid_voltage_lineedit = nullptr;
QComboBox* domain_menu= nullptr;
 QVBoxLayout* vbox4=nullptr;

QButtonGroup *radiogroup1 = nullptr;
QRadioButton *adaptive_radio= nullptr;
QRadioButton *manual_radio= nullptr;
QRadioButton *offset_radio= nullptr;
 QVBoxLayout* vbox5=nullptr;

 QPushButton *apply_svid_button=nullptr;
  QPushButton *maxout_button=nullptr;
   QVBoxLayout* vbox6=nullptr;

   QHBoxLayout* hbox2=nullptr;
   QSpacerItem* spacer3=nullptr;
   QFrame *line2=nullptr;
    QVBoxLayout* vbox7=nullptr;
//

    QLabel *ratio_limit_label = nullptr;
    QLabel *act_cores_label = nullptr;
    QLabel *atom_ratio_label = nullptr;
    QLabel *act_atom_label = nullptr;
    vector<vector< QLineEdit *>> ratio_limit_vec;
  QPushButton *apply_ratios_button=nullptr;
//
   QLabel *synch_allcores_label = nullptr;
   QLabel *synch_allatom_label = nullptr;
    QLabel *fixed_ring_label = nullptr;
    QPushButton *apply_core_button=nullptr;
     QPushButton *apply_ring_button=nullptr;

     QLabel *bclk_label = nullptr;
     QLabel *pcie_label = nullptr;
    QPushButton *apply_bclk_button=nullptr;
    QLineEdit *synch_allcores_lineedit = nullptr;
    QLineEdit *synch_allatom_lineedit = nullptr;
    QLineEdit *fixed_ring_lineedit = nullptr;
    QLineEdit *bclk_lineedit = nullptr;
    QLineEdit *pcie_lineedit = nullptr;

public slots:
     void   apply_vrm_button_pressed();
     void apply_svid_button_pressed();
    void maxout_button_pressed();
   void apply_ratios_button_pressed();
   void apply_core_button_pressed();
   void apply_ring_button_pressed();
  void  apply_bclk_button_pressed();

signals:
      void send_cmd(vector<char> &vec);
*/

};


class amd1 : public QWidget
{
    Q_OBJECT

public:
    explicit amd1(QWidget *parent = 0);
      ~amd1();
public:
    QLabel* vrm_voltage_label = nullptr;
    QLabel* vrm_loadline_label = nullptr;

    QLineEdit* vrm_voltage_lineedit = nullptr;
    QLineEdit* vrm_loadline_lineedit = nullptr;
    QPushButton* apply_vrm_button= nullptr;
QFrame*  line1 =nullptr;

QLineEdit* allcore_freq_lineedit = nullptr;
QLineEdit* vid_lineedit = nullptr;
QPushButton* apply_it= nullptr;
QPushButton* apply_ccx= nullptr;

    vector<QLabel *> qlabel_vec1;
    vector<QLineEdit*> core_ratio_vec;

    QSpacerItem* seperator=nullptr;
    QSpacerItem* spacer1=nullptr;
    QVBoxLayout* mainvbox=nullptr;

    QLabel* allco= nullptr;
    QLabel* vider= nullptr;


signals:

     void send_cmd(vector<char>&to_send);
public slots:
 void applyvr();
 void applyvid();
void applyccx();
};


QT_USE_NAMESPACE

class ChatServer;
class ChatClient;

//! [declaration]
class Chat : public QDialog
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
      ChatServer *server;
      bool doing_osc{false};
     bool got_osc=false;
    bool got_arch{false};
 QBluetoothDeviceDiscoveryAgent *discoveryAgent=nullptr;
 //RemoteSelector2* connector_widget=nullptr;
 QList<QBluetoothAddress> address_vec;
 QList <QBluetoothUuid> uuid_vec;
 QBluetoothSocket* socket1=nullptr;;
 int devnumber_to_connect=0;
 bool this_is_intel{true};
intel1* intel1tab=nullptr;
intel2* intel2tab=nullptr;
amd1* amd1tab=nullptr;
mainwindow4* osctab=nullptr;
volatile bool keep_monitor{true};
QScrollArea* m_scrollArea=nullptr;
signals:
    void sendMessage(const QString &message);
void killme();
public slots:
    void send_cmd(vector<char>& vec);
    void radioButton_2_changed(bool checked);
    void quit_clicked();
void  page_changed();
    void widgets_got_arch();
     void startClient();
    void device_selected(int devnum, QList<QBluetoothAddress> addy, QList <QBluetoothUuid> udy);
    void connectClicked();
    void sendClicked();
    void showMessage2(int length);

    void showMessage(const QString &sender, const QString &message);

    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

    void newAdapterSelected();
void test_window();

private:
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;
    Ui_Chat *ui;


    QList<ChatClient *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
};
//! [declaration]


#endif
