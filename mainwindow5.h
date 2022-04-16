#ifndef MAINWINDOW5_H
#define MAINWINDOW5_H

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
#include "samplingthread.h"
#include "plot.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include "filter.h"

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



namespace Ui {
class mainwindow5;
}

class mainwindow5 : public QWidget
{
    Q_OBJECT

public:
     explicit mainwindow5 (vector<vector<double>> &qvec, QWidget *parent = nullptr);
     explicit mainwindow5 (vector<vector<double>> &qvec, vector<vector<double>> &qvec2,QWidget *parent = nullptr);
 Plot *d_plot;

 ~mainwindow5();
public slots:

//void closeEvent(QCloseEvent*);

};




#endif // MAINWINDOW5_H
