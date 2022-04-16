#ifndef MAINWINDOW4_H
#define MAINWINDOW4_H

#include <qwidget.h>

#include <QMainWindow>
//#include "chart.h"
#include <QtCharts/QChartView>
#include <vector>
#include <array>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QtGui/QtGui>
//#include "usb_osc_class.h"
#include "samplingthread.h"
#include "plot.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include "filter.h"
#include <QGraphicsPathItem>
#include <QMutex>
#include <QGraphicsScene>
#include "chatserver.h"
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include "graphicsline.h"


using namespace std;

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

//class Callout;

QT_CHARTS_USE_NAMESPACE



class Plot;


using namespace std;

extern int thecomtouse;
extern volatile int ch1_filter;
extern volatile int ch2_filter;
extern volatile int ch3_filter;
extern volatile bool recordthis;

namespace Ui {
class mainwindow4;
}

class mainwindow4 : public QWidget
{
    Q_OBJECT

public:
     explicit mainwindow4 ( ChatServer *server1, QWidget *parent = nullptr);
//
      QHBoxLayout *mylayout = nullptr;
    void stop();
    bool started=false;
    bool virgin=true;
    bool virgin2=true;
    bool virgin3=true;

    bool channel2_started=false;
    bool channel3_started=false;

    ChatServer *server;
bool just_resized=false;
    int current_factor=0;
    int d_timerId;
    int d_timerId2;
    int d_timerId3;
    qint64 myviewport_width=1600;
    qint64 myviewport_height=1000;
    qint64 time_duration=10000;
double current_amplitude=2000.0;

QGraphicsSimpleTextItem* upper_scale=nullptr;
QGraphicsSimpleTextItem* lower_scale=nullptr;
QGraphicsSimpleTextItem* middle_scale=nullptr;
QGraphicsSimpleTextItem* upper_middle_scale=nullptr;
QGraphicsSimpleTextItem* lower_middle_scale=nullptr;

QPen pen1;

QPen pen2;

QPen pen3;

qreal current_x1=0;
qreal current_y1=0;

qreal current_x2=0;
qreal current_y2=0;

qreal current_x3=0;
qreal current_y3=0;


GraphicsLineItem* gitem;
GraphicsLineItem* gitem2;
GraphicsLineItem* gitem3;

 QMutex *Sampling_mutex;
QGraphicsScene *scene;
  Plot *d_plot;
protected:

    virtual void timerEvent( QTimerEvent * ) override;

public:

  QFuture<void>* pWatcher;
    void start();
    QLabel *label3a;

    QLabel *label4;

    QLabel *label5;

    QLabel *label6;

    QPushButton* pause;
    QPushButton* clear_stats_but;
QPushButton* compare_but;

    QPushButton* record_but;
     QPushButton* playback_but;
 QPushButton* getacll_but;
    QCheckBox* checkbox1;
    QCheckBox* checkbox2;
    QCheckBox* checkbox3;
    QCheckBox* checkbox4;
    QLabel* status1;

    QLabel* label1;

    QLabel* label2;

    QLabel* label3;

    QLabel* label2a;




    QRadioButton* below_but;
    QRadioButton* above_but;

    QRadioButton* trigger_ch1_but;
    QRadioButton* trigger_ch2_but;
    QRadioButton* trigger_ch3_but;



    QLineEdit* triggerline;

    QLineEdit* minline;
    QLineEdit* maxline;

    QLineEdit* mculine;
    QLabel* mculabel;
    double current_sample_rate{1};

    QPushButton* filter_but;
     filter *filter1;




 ~mainwindow4();
public slots:
     void can_start();
     void view_resized();
     void cleanme();
     void updateCurve();
      void updateCurve2();
       void updateCurve3();
       void  checkbox1_changed(int state);
         void  checkbox2_changed(int state);
           void  checkbox3_changed(int state);
void run_button_clicked();
     //

bool isClosed();
void closeEvent(QCloseEvent*) override ;
void resizeEvent(QResizeEvent *event) override ;

Q_SIGNALS:

private:

};



#endif // MAINWINDOW4_H
