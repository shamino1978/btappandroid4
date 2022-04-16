#ifndef PLOT_H
#define PLOT_H

#include <QtCharts/QChartView>
#include <vector>
#include <array>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>

#include <samplingthread.h>
#include <QtConcurrent>
#include <QFuture>


#include <QFrame>
#include <QGraphicsView>
#include <QElapsedTimer>
#include <QTimer>
#include <QCheckBox>
#include <QPushButton>


QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class Plot;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(Plot *v) : QGraphicsView(), view(v) {
      //  viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
        //setDragMode(ScrollHandDrag);
    }

     bool viewportEvent(QEvent *event) override;
     qreal totalScaleFactor;
     qreal actual_scale;
signals:
     void scale_me();
protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

private:
    Plot *view;
};


extern volatile bool channel1;
extern volatile bool channel2;
extern volatile bool channel3;


class Plot: public QFrame
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);

    QGraphicsView *view() const;
GraphicsView *graphicsView;
QGraphicsScene *scene2;

QCheckBox* channel1_cb= nullptr;
QCheckBox* channel2_cb=nullptr;
QCheckBox* channel3_cb=nullptr;
QPushButton* start_button=nullptr;
QTimer* m_resizeTimer=new QTimer(this);
QPushButton* quit_button=nullptr;
QPushButton* zoomin_but=nullptr;
QPushButton* zoomout_but=nullptr;
signals:

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
void resizeDone( ) ;
void scale_me();
void do_zoomIn();
void do_zoomOut();
private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void rotateLeft();
    void rotateRight();

private:
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QPushButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;

signals:
   void resized();
public:
void resizeEvent(QResizeEvent *event) override;

    virtual ~Plot();

};


#endif // PLOT_H
