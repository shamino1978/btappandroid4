/********************************************************************************
** Form generated from reading UI file 'remoteselector2.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTESELECTOR2_H
#define UI_REMOTESELECTOR2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemoteSelector2
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *status;
    QListWidget *devices_list;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *RemoteSelector2)
    {
        if (RemoteSelector2->objectName().isEmpty())
            RemoteSelector2->setObjectName(QString::fromUtf8("RemoteSelector2"));
        RemoteSelector2->resize(437, 389);
        verticalLayout = new QVBoxLayout(RemoteSelector2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        status = new QLabel(RemoteSelector2);
        status->setObjectName(QString::fromUtf8("status"));

        verticalLayout->addWidget(status);

        devices_list = new QListWidget(RemoteSelector2);
        devices_list->setObjectName(QString::fromUtf8("devices_list"));

        verticalLayout->addWidget(devices_list);

        pushButton = new QPushButton(RemoteSelector2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(RemoteSelector2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);


        retranslateUi(RemoteSelector2);

        QMetaObject::connectSlotsByName(RemoteSelector2);
    } // setupUi

    void retranslateUi(QDialog *RemoteSelector2)
    {
        RemoteSelector2->setWindowTitle(QCoreApplication::translate("RemoteSelector2", "Available chat services", nullptr));
        status->setText(QCoreApplication::translate("RemoteSelector2", "Scanning...", nullptr));
        pushButton->setText(QCoreApplication::translate("RemoteSelector2", "Connect", nullptr));
        pushButton_2->setText(QCoreApplication::translate("RemoteSelector2", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoteSelector2: public Ui_RemoteSelector2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTESELECTOR2_H
