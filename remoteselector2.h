#ifndef REMOTESELECTOR2_H
#define REMOTESELECTOR2_H



#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include <QList>
#include <QtBluetooth/qbluetoothdeviceinfo.h>

using namespace std;
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
    class RemoteSelector2;
}
QT_END_NAMESPACE

class RemoteSelector2: public QDialog
{
    Q_OBJECT
    public:
QList<QBluetoothAddress> address_vec;

QList <QBluetoothUuid> uuid_vec;

    explicit RemoteSelector2(QWidget *parent = nullptr);
    ~RemoteSelector2();
   // QList<QBluetoothDeviceInfo> devices_detected;
signals:
    void selected_device(int devnum, QList<QBluetoothAddress> add_v, QList <QBluetoothUuid> uu_v);
public slots:
   void deviceDiscovered(const QBluetoothDeviceInfo &device);

private slots:
    void on_pushButton_clicked();

private:
    Ui::RemoteSelector2 *ui;
};


#endif // REMOTESELECTOR2_H
