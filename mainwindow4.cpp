#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

#include <sstream>
#include <array>
#include <cmath>
#include <unistd.h>
#include <iomanip>
#include <stdlib.h>
#include <memory>
#include <QtWidgets>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QScreen>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>

#include "mainwindow4.h"


#include <QEventLoop>

#include <QTextStream>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QList>
#include <QLabel>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "samplingthread.h"
#include <qapplication.h>

#include "plot.h"


#include <qlabel.h>
#include <qlayout.h>


#include "filter.h"
#include "ui_filter.h"

#include "mainwindow5.h"
#include <QResizeEvent>

volatile bool _isClosed{false};


volatile double osc_lower_thresh_hit{ 0 };
volatile double osc_upper_thresh_hit{ 0 };
volatile double osc_abs_lowest{ 3300 };
volatile double osc_abs_highest{ 0 };

volatile long int osc_gettimes{0};

volatile double osc_lower_thresh{1000};
volatile double osc_upper_thresh{1400};

volatile double osc_lower_per;
volatile double osc_upper_per;

volatile int trigger_voltage{-1};

int thecomtouse{3};
double mcu_sample_rate{1};


volatile int ch1_filter{3300};
volatile int ch2_filter{3300};
volatile int ch3_filter{3300};



template <class T>
void save_file_clean2(std::vector<std::vector<T> > buff, string fname) {
    ofstream outputFile1;
    outputFile1.open(fname);
    for (long long unsigned int i{ 0 }; i < buff.size(); i++) {
        for (long long unsigned int j{ 0 }; j < buff[0].size(); j++) {
            if (j != buff[0].size() - 1) {
                outputFile1 << buff[i][j] << ",";
            }
            else {
                outputFile1 << buff[i][j];
            }
        }

        outputFile1 << "\n";
    }
    outputFile1.close();
}


std::vector<std::vector<double> > csvreaddoub2 (std::string filename){
std::vector<std::vector<double> > values;
   std::ifstream fin(filename);
   for (std::string line; std::getline(fin, line); )
   {
       std::replace(line.begin(), line.end(), ',', ' ');
       std::istringstream in(line);
       values.push_back(
           std::vector<double>(std::istream_iterator<double>(in),
                               std::istream_iterator<double>()));
   }
return values;
}


filter::filter(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::filter){
    ui->setupUi(this);
    this->setWindowTitle("Filter");
    ui->pushButton->setStyleSheet("color: black");
    ui->lineEdit->setStyleSheet("color: black");
    ui->lineEdit_2->setStyleSheet("color: black");
     ui->lineEdit_3->setStyleSheet("color: black");
    ui->lineEdit->setText(QString::number(ch1_filter));
     ui->lineEdit_2->setText(QString::number(ch2_filter));
      ui->lineEdit_3->setText(QString::number(ch3_filter));
}

filter::~filter(){
    delete ui;
};

void filter::on_pushButton_clicked(){
if(!ui->lineEdit->text().isEmpty()){
    ch1_filter=ui->lineEdit->text().toUInt();
}
if(!ui->lineEdit_2->text().isEmpty()){
    ch2_filter=ui->lineEdit_2->text().toUInt();
}
if(!ui->lineEdit_3->text().isEmpty()){
    ch3_filter=ui->lineEdit_3->text().toUInt();
}
QMessageBox msgBox;
msgBox.setBaseSize(QSize(600, 600));
msgBox.setStyleSheet("color: black");
msgBox.information(this, "Set Filter", "<FONT COLOR='#000000'>Applied!</FONT>");
}

//static volatile bool getacll{false};

void reset_all(){
 _isClosed=false;
  osc_lower_thresh_hit=0;
  osc_upper_thresh_hit=0;
  osc_abs_lowest= 3300 ;
    osc_abs_highest=0;
   osc_gettimes=0;
   osc_lower_thresh=1000;
 osc_upper_thresh=1400;
  osc_lower_per=0;
  osc_upper_per=0;

trigger_voltage=-1;
 channel1=true;
channel2=false;
channel3=false;
data_index_counter=0;
data_index_counter2=0;
data_index_counter3=0;
osc_data_vec1.clear();
osc_data_vec2.clear();
osc_data_vec3.clear();

osc_data_vec1b_size=0;
osc_data_vec2b_size=0;
osc_data_vec3b_size=0;

osc_trigger_vec1.clear();
osc_trigger_vec2.clear();
osc_trigger_vec3.clear();
osc_trigger_vec1b_size=0;
osc_trigger_vec2b_size=0;
osc_trigger_vec3b_size=0;

triggered=false;
trigger=false;
runit=true;
trigger_fall=false;
trigger_rise=false;

trigger_channel=1;
trigger_record_times=0;
triggered_index=0;

ch1_filter=3300;
ch2_filter=3300;
ch3_filter=3300;

recordthis=false;
ch1_recordvec.clear();
ch2_recordvec.clear();
ch3_recordvec.clear();

}


mainwindow4::mainwindow4( ChatServer *server1,QWidget *parent ):
   QWidget(parent),
   server(server1)
{
    this->setWindowTitle("USB Oscilloscope");

}


void mainwindow4::updateCurve(){

    qint64 x=d_clock.elapsed();

    double factor=static_cast<double>(x)/static_cast<double>(time_duration);
    if (factor>current_factor){
        current_factor++;
        virgin=true;
        virgin2=true;
        virgin3=true;

        x=0;
        while(!Sampling_mutex->tryLock()){

        }

        QList<QGraphicsItem*> all = scene->items();
        for (int i = 0; i < all.size(); i++)
        {
            QGraphicsItem *gi = all[i];
            if(gi->parentItem()==NULL) {
                delete gi;
            }
        }
          scene->update();
        qApp->processEvents();

    upper_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/1000.0,'f', 2));
  lower_scale=new QGraphicsSimpleTextItem(QString::number(0.0,'f', 2));
  middle_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/2000.0,'f', 2));
  upper_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude*3.0)/4000.0,'f', 2));
  lower_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude)/4000.0,'f', 2));

  scene->addItem(upper_scale);
  scene->addItem(lower_scale);
  scene->addItem(middle_scale);
  scene->addItem(upper_middle_scale);
    scene->addItem(lower_middle_scale);
    lower_scale->setPos(5,myviewport_height-5);
    middle_scale->setPos(5,myviewport_height/2.0);
    upper_scale->setPos(5 ,5);
    lower_middle_scale->setPos(5 ,(myviewport_height*3.0)/4.0);
    upper_middle_scale->setPos(5 ,(myviewport_height)/4.0);
Sampling_mutex->unlock();
    }else{
        x=x % time_duration;
        factor=static_cast<double>(x)/static_cast<double>(time_duration);
        factor*=myviewport_width;
        x=factor;

    }

    bool skip=false;
    if(osc_data_vec1b_size<=data_index_counter){
            skip=true;
      }

    if(!skip){

    int y=osc_data_vec1b[data_index_counter];
    data_index_counter++;

    if(data_index_counter==20400){
        osc_data_vec1b_size=0;
        data_index_counter=0;
    }

    double fraction=(current_amplitude -static_cast<double>(y))/current_amplitude;
    y=fraction*static_cast<double>(myviewport_height);
    if(virgin){
       current_x1=x;
        current_y1=y;
        virgin=false;
    }
    gitem=new GraphicsLineItem(pen1,current_x1,x,current_y1,y );
    scene->addItem(gitem);
    current_x1=x;
    current_y1=y;
   // scene->update();

    }



}


void mainwindow4::updateCurve2(){
    qint64 x=d_clock.elapsed();

    double factor=static_cast<double>(x)/static_cast<double>(time_duration);
    if (factor>current_factor){
        current_factor++;
        virgin=true;
        virgin2=true;
        virgin3=true;

        x=0;
        while(!Sampling_mutex->tryLock()){

        }

        QList<QGraphicsItem*> all = scene->items();
        for (int i = 0; i < all.size(); i++)
        {
            QGraphicsItem *gi = all[i];
            if(gi->parentItem()==NULL) {
                delete gi;
            }
        }
          scene->update();
        qApp->processEvents();

    upper_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/1000.0,'f', 2));
  lower_scale=new QGraphicsSimpleTextItem(QString::number(0.0,'f', 2));
  middle_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/2000.0,'f', 2));
  upper_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude*3.0)/4000.0,'f', 2));
  lower_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude)/4000.0,'f', 2));

  scene->addItem(upper_scale);
  scene->addItem(lower_scale);
  scene->addItem(middle_scale);
  scene->addItem(upper_middle_scale);
    scene->addItem(lower_middle_scale);
    lower_scale->setPos(5,myviewport_height-5);
    middle_scale->setPos(5,myviewport_height/2.0);
    upper_scale->setPos(5 ,5);
    lower_middle_scale->setPos(5 ,(myviewport_height*3.0)/4.0);
    upper_middle_scale->setPos(5 ,(myviewport_height)/4.0);
Sampling_mutex->unlock();
    }else{
        x=x % time_duration;
        factor=static_cast<double>(x)/static_cast<double>(time_duration);
        factor*=myviewport_width;
        x=factor;

    }

    bool skip=false;
    if(osc_data_vec2b_size<=data_index_counter2){
            skip=true;
      }

    if(!skip){

    int y=osc_data_vec2b[data_index_counter2];
    data_index_counter2++;

    if(data_index_counter2==20400){
        osc_data_vec2b_size=0;
        data_index_counter2=0;
    }

    double fraction=(current_amplitude -static_cast<double>(y))/current_amplitude;
    y=fraction*static_cast<double>(myviewport_height);
    if(virgin2){
       current_x2=x;
        current_y2=y;
        virgin2=false;
    }
    gitem2=new GraphicsLineItem(pen2,current_x2,x,current_y2,y );
    scene->addItem(gitem2);
    current_x2=x;
    current_y2=y;
   // scene->update();


}


}


void mainwindow4::updateCurve3(){
    qint64 x=d_clock.elapsed();
    double factor=static_cast<double>(x)/static_cast<double>(time_duration);
    if (factor>current_factor){
        current_factor++;
        virgin=true;
        virgin2=true;
        virgin3=true;

        x=0;
        while(!Sampling_mutex->tryLock()){

        }

        QList<QGraphicsItem*> all = scene->items();
        for (int i = 0; i < all.size(); i++)
        {
            QGraphicsItem *gi = all[i];
            if(gi->parentItem()==NULL) {
                delete gi;
            }
        }
          scene->update();
        qApp->processEvents();

    upper_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/1000.0,'f', 2));
  lower_scale=new QGraphicsSimpleTextItem(QString::number(0.0,'f', 2));
  middle_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/2000.0,'f', 2));
  upper_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude*3.0)/4000.0,'f', 2));
  lower_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude)/4000.0,'f', 2));

  scene->addItem(upper_scale);
  scene->addItem(lower_scale);
  scene->addItem(middle_scale);
  scene->addItem(upper_middle_scale);
    scene->addItem(lower_middle_scale);
    lower_scale->setPos(5,myviewport_height-5);
    middle_scale->setPos(5,myviewport_height/2.0);
    upper_scale->setPos(5 ,5);
    lower_middle_scale->setPos(5 ,(myviewport_height*3.0)/4.0);
    upper_middle_scale->setPos(5 ,(myviewport_height)/4.0);
Sampling_mutex->unlock();
    }else{
        x=x % time_duration;
        factor=static_cast<double>(x)/static_cast<double>(time_duration);
        factor*=myviewport_width;
        x=factor;

    }

    bool skip=false;
    if(osc_data_vec3b_size<=data_index_counter3){
            skip=true;
      }

    if(!skip){

    int y=osc_data_vec3b[data_index_counter3];
    data_index_counter3++;

    if(data_index_counter3==20400){
        osc_data_vec3b_size=0;
        data_index_counter3=0;
    }

    double fraction=(current_amplitude -static_cast<double>(y))/current_amplitude;
    y=fraction*static_cast<double>(myviewport_height);
    if(virgin3){
       current_x3=x;
        current_y3=y;
        virgin3=false;
    }
    gitem3=new GraphicsLineItem(pen3,current_x3,x,current_y3,y );
    scene->addItem(gitem3);
    current_x3=x;
    current_y3=y;
   // scene->update();


}

}

void mainwindow4::timerEvent( QTimerEvent *event )
{
    if(runit){
    if ( event->timerId() == d_timerId && channel1)
    {

        updateCurve();

    }
    else if ( event->timerId() == d_timerId2 && channel2)
    {

       updateCurve2();


    }

    else if ( event->timerId() == d_timerId3 && channel3 )
    {

       updateCurve3();

    }
        }
 }



mainwindow4:: ~mainwindow4(){
     runit=false;
  delete upper_scale;
  delete lower_scale;
   delete middle_scale;
   delete upper_middle_scale;
 delete lower_middle_scale;

  delete gitem;
  delete gitem2;
 delete gitem3;

   delete Sampling_mutex;
 delete scene;
    delete d_plot;


    delete pWatcher;
    delete label3a;

    delete label4;

    delete label5;

   delete label6;
    delete pause;
    delete clear_stats_but;
    delete checkbox1;
    delete checkbox2;
    delete checkbox3;
    delete checkbox4;
    delete status1;
    delete label1;
    delete label2;
    delete label3;
    delete label2a;
    delete below_but;
    delete above_but;
    delete trigger_ch1_but;
    delete trigger_ch2_but;
    delete trigger_ch3_but;
    delete triggerline;
    delete minline;
    delete maxline;


      delete d_plot;
    delete mculine;
    delete mculabel;
    delete filter_but;
     delete  filter1;
       delete compare_but;





}

bool mainwindow4::isClosed()
{
    return _isClosed;
}

void mainwindow4::closeEvent(QCloseEvent*)
{
    // delete pWatcher;

    data_index_counter=0;
    osc_data_vec1.clear();
    data_index_counter2=0;
    osc_data_vec2.clear();
    data_index_counter3=0;
    osc_data_vec3.clear();
    _isClosed = true;
}

void mainwindow4::checkbox1_changed(int state){
    if(state==0){
        channel1=false;
        scene->removeItem(gitem);
        d_plot->update();
         scene->update();
         vector<char>sender={11,1,123};
         server->sendMessage2(sender);
    }
    else{
        channel1=true;

         vector<char>sender={22,1,123};
         server->sendMessage2(sender);
    }
}


void mainwindow4::checkbox2_changed(int state){
    if(state==0){
        channel2=false;
    scene->removeItem(gitem2);
    d_plot->update();
     scene->update();
     vector<char>sender={11,2,123};
     server->sendMessage2(sender);
    }
    else{
        channel2=true;
        if(!channel2_started){
            d_timerId2 = startTimer(1);
            channel2_started=true;
      }

         vector<char>sender={22,2,123};
         server->sendMessage2(sender);
    }
}


void mainwindow4::checkbox3_changed(int state){
    if(state==0){
        channel3=false;
        scene->removeItem(gitem3);
        d_plot->update();
         scene->update();
         vector<char>sender={11,3,123};
         server->sendMessage2(sender);
    }
    else{
        channel3=true;
        if(!channel3_started){
             d_timerId3 = startTimer(1);
             channel3_started=true;
        }


         vector<char>sender={22,3,123};
         server->sendMessage2(sender);
    }
}


void mainwindow4::run_button_clicked(){
    if(trigger_voltage==-1){
    if(runit){
        runit=false;
        d_plot->start_button->setText("Start");

    }else{
        osc_lower_thresh_hit= 0;
        osc_upper_thresh_hit= 0 ;
        osc_abs_lowest= 3300 ;
        osc_abs_highest= 0 ;
        osc_gettimes=0;
      osc_lower_per=0;
     osc_upper_per=0;

        runit=true;
        d_plot->start_button->setText("Stop");

    }

    }else if(!runit){
        trigger=true;


    }
}

void mainwindow4::stop(){
    //started=false;
    runit=false;
  /*  killTimer( d_timerId );
    killTimer( d_timerId2 );
    killTimer( d_timerId3 );

    QList<QGraphicsItem*> all = scene->items();
        for (int i = 0; i < all.size(); i++)
        {
            QGraphicsItem *gi = all[i];
            if(gi->parentItem()==NULL) {
                delete gi;
            }
        }
        mylayout->removeWidget(d_plot);*/

}

void mainwindow4::start()
{
    reset_all();

   //pWatcher =new  QFuture<void> ;
   //this->setPalette( Qt::darkGray );
Sampling_mutex=new QMutex;

    scene = new QGraphicsScene(this);

   d_plot = new Plot( this );

    d_plot->view()->setScene(scene);


     connect(d_plot->start_button,SIGNAL(clicked()), this,  SLOT( run_button_clicked() ));


      mylayout = new QHBoxLayout;
      mylayout->addWidget(d_plot);
     setLayout(mylayout);
     myviewport_width=d_plot->graphicsView->viewport()->width();
     myviewport_height=d_plot->graphicsView->viewport()->height();

 pen1.setColor(Qt::red);
 pen2.setColor(Qt::blue);
  pen3.setColor(Qt::darkGreen);
//gitem=new GraphicsLineItem(pen1);

  upper_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/1000.0,'f', 2));
lower_scale=new QGraphicsSimpleTextItem(QString::number(0.0,'f', 2));
middle_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/2000.0,'f', 2));
upper_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude*3.0)/4000.0,'f', 2));
lower_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude)/4000.0,'f', 2));

//scene->addItem(gitem);

 // scene->addItem(item);
//scene->addItem(item2);
//scene->addItem(item3);
scene->addItem(upper_scale);
scene->addItem(lower_scale);
scene->addItem(middle_scale);
scene->addItem(upper_middle_scale);
scene->addItem(lower_middle_scale);
lower_scale->setPos(5,myviewport_height-5);
middle_scale->setPos(5,myviewport_height/2.0);
upper_scale->setPos(5 ,5);
lower_middle_scale->setPos(5 ,(myviewport_height*3.0)/4.0);
upper_middle_scale->setPos(5 ,(myviewport_height)/4.0);
 // d_plot->graphicsView->fitInView( item);

connect(d_plot,SIGNAL(resized()), this,  SLOT( view_resized() ));
QTimer* m_resizeTimer=new QTimer(this);
m_resizeTimer->setSingleShot(true);
connect(m_resizeTimer, SIGNAL(timeout()), SLOT(can_start()));
m_resizeTimer->start(500);


}

void mainwindow4::can_start(){
d_clock.start();
d_timerId = startTimer(1);
started=true;
connect( d_plot->channel1_cb, SIGNAL( stateChanged(int ) ),
    this, SLOT( checkbox1_changed( int ) ) );

connect( d_plot->channel2_cb, SIGNAL( stateChanged(int ) ),
    this, SLOT( checkbox2_changed( int ) ) );

connect( d_plot->channel3_cb, SIGNAL( stateChanged(int ) ),
    this, SLOT( checkbox3_changed( int ) ) );
}
void mainwindow4::resizeEvent(QResizeEvent *event)  {


         //  myviewport_width=d_plot->graphicsView->viewport()->width();
          // myviewport_height=d_plot->graphicsView->viewport()->height();
         //  d_plot->graphicsView->ensureVisible( item,  0,   0);

      //cleanme();
}

void mainwindow4::view_resized()  {

  myviewport_width=d_plot->graphicsView->viewport()->width();
          myviewport_height=d_plot->graphicsView->viewport()->height();
  //  d_plot->graphicsView->ensureVisible( item,  0,   0);

     cleanme();
}


void mainwindow4:: cleanme(){

    virgin=true;

    while(!Sampling_mutex->tryLock()){

    }
    QList<QGraphicsItem*> all = scene->items();
    for (int i = 0; i < all.size(); i++)
    {
        QGraphicsItem *gi = all[i];
        if(gi->parentItem()==NULL) {
            delete gi;
        }
    }

//gitem=new GraphicsLineItem( pen1);

upper_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/1000.0,'f', 2));
lower_scale=new QGraphicsSimpleTextItem(QString::number(0.0,'f', 2));
middle_scale=new QGraphicsSimpleTextItem(QString::number(current_amplitude/2000.0,'f', 2));
upper_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude*3.0)/4000.0,'f', 2));
lower_middle_scale=new QGraphicsSimpleTextItem(QString::number((current_amplitude)/4000.0,'f', 2));

//scene->addItem(gitem);
  // scene->addItem(item);
//scene->addItem(item2);
//scene->addItem(item3);
scene->addItem(upper_scale);
scene->addItem(lower_scale);
scene->addItem(middle_scale);
scene->addItem(upper_middle_scale);
scene->addItem(lower_middle_scale);
lower_scale->setPos(5,myviewport_height-5);
middle_scale->setPos(5,myviewport_height/2.0);
upper_scale->setPos(5 ,5);
lower_middle_scale->setPos(5 ,(myviewport_height*3.0)/4.0);
upper_middle_scale->setPos(5 ,(myviewport_height)/4.0);
scene->update();
qApp->processEvents();


Sampling_mutex->unlock();


    /*  QList<QGraphicsItem*> all = scene->items();
    for (int i = 0; i < all.size(); i++)
    {
        QGraphicsItem *gi = all[i];
        if(gi->parentItem()==NULL) {
            delete gi;
        }
    }*/
/*
while(!Sampling_mutex->tryLock()){

}
    scene->removeItem(item);
    scene->removeItem(item2);
    scene->removeItem(item3);

    scene->update();
    delete item;
    delete item2;
    delete item3;

    item=new QGraphicsPathItem;
    item2=new QGraphicsPathItem;
    item3=new QGraphicsPathItem;

    qApp->processEvents();
    scene->update();
    just_resized=true;
d_plot->graphicsView->ensureVisible( item,  0,   0);
//QPointF center = d_plot->graphicsView->mapToScene(d_plot->graphicsView->viewport()->rect().center());
//center.setY(center.y()*1);
//center.setY(center.y()/3);

//QPointF pos(center) ;
//d_plot->graphicsView->centerOn(center);

//d_plot->graphicsView->fitInView( item);

  //   item=new QGraphicsPathItem;
//item2=new QGraphicsPathItem;
//item3=new QGraphicsPathItem;
   // scene->addItem(item);
    //scene->addItem(item2);
     //scene->addItem(item3);

Sampling_mutex->unlock();*/
}
