
#include "plot.h"
//#include "curvedata.h"
#include "signaldata.h"
#include <qevent.h>


//#include "QString"
#include <QPen>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <QMouseEvent>
#include <QGraphicsView>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

using namespace std;


volatile bool channel1;
volatile bool channel2;
volatile bool channel3;


#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif


bool GraphicsView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {

            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
            if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.
                totalScaleFactor *= currentScaleFactor;
                currentScaleFactor = 1;
            }
           // setTransform(QTransform().scale(totalScaleFactor * currentScaleFactor,
             //                               totalScaleFactor * currentScaleFactor));
            actual_scale=totalScaleFactor * currentScaleFactor;
            emit scale_me();
            return false;
        }
        //return true;
    }
    default:
        break;
    }
    return QGraphicsView::viewportEvent(event);
}


Plot::Plot( QWidget *parent)
    : QFrame(parent)

{
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);

    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);


    graphicsView->setSceneRect(0, 0, 0, 0);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     graphicsView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
     //graphicsView->setRenderHint(QPainter::Antialiasing, true);
    //if(QGLFormat::hasOpenGL()){
      //   graphicsView->setViewport( new QGLWidget(QGLFormat(QGL::SampleBuffers)));
        // graphicsView->setRenderHint(QPainter::Antialiasing, true);
    //}

     graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);


    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksLeft);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QPushButton("Reset");

    //resetButton = new QToolButton;
    //resetButton->setText(tr("Reset"));
   // resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;

    label2 = new QLabel(tr("Pointer Mode"));
    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("Select"));
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(true);
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("Antialiasing"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(false);
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
    //openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
    openGlButton->setEnabled(false);
#endif
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);


    labelLayout->addWidget(label2);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addStretch();
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(openGlButton);
    labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
   // topLayout->addLayout(labelLayout, 0, 0);
   // topLayout->addWidget(graphicsView,1, 0);
    //topLayout->addLayout(zoomSliderLayout, 1, 1);
    //topLayout->addLayout(rotateSliderLayout, 2, 0);
    //topLayout->addWidget(resetButton, 2, 1);
 topLayout->addWidget(graphicsView,0, 0);
//topLayout->addLayout( zoomSliderLayout,0, 1);
    QHBoxLayout *hbox=new QHBoxLayout;
   // hbox->addLayout(topLayout);
   // hbox->addWidget(graphicsView);

    QVBoxLayout *vbox=new QVBoxLayout;
    channel1_cb=new QCheckBox("CH1");
     channel2_cb=new QCheckBox("CH2");
     channel3_cb=new QCheckBox("CH3");
    start_button=new QPushButton("Stop");
  zoomin_but=new QPushButton("ZoomIn");
    zoomout_but=new QPushButton("ZoomOut");

quit_button=new QPushButton("Quit");
 vbox->addWidget(channel1_cb);
 vbox->addWidget(channel2_cb);
 vbox->addWidget(channel3_cb);
    vbox->addWidget(start_button);
    vbox->addWidget(resetButton);
     vbox->addWidget(zoomin_but);
      vbox->addWidget(zoomout_but);
    vbox->addWidget(quit_button);
   // vbox->addWidget(channel1_cb);
    //vbox->addWidget(channel2_cb);
    //vbox->addWidget(channel3_cb);
    topLayout->addLayout(vbox,0,1);
    topLayout->setRowStretch(0, 100);

    topLayout->setColumnStretch(0, 100);
//topLayout->setColumnStretch(1, 1);
//topLayout->setColumnStretch(2, 1);
    //topLayout->setRowStretch(0, 1);


     //hbox->addLayout(vbox);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
   // connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));

    connect(zoomin_but, SIGNAL(clicked()), this, SLOT(do_zoomIn()));
    connect(zoomout_but, SIGNAL(clicked()), this, SLOT(do_zoomOut()));


    //connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    //connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
     //       this, SLOT(setResetButtonEnabled()));
   // connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
   //         this, SLOT(setResetButtonEnabled()));
   // connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
   // connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
   // connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
   // connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
   // connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
   // connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
   // connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
   // connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
   // connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
    channel1_cb->setChecked(true);

    // connect(graphicsView, SIGNAL(scale_me()), this, SLOT(scale_me()));

    setupMatrix();
  }

Plot::~Plot(){

delete graphicsView;

    delete scene2;


    delete channel1_cb;
        delete  channel2_cb;
delete channel3_cb;
delete  start_button;
delete  m_resizeTimer;
delete  quit_button;
delete  zoomin_but;
delete  zoomout_but;

 delete label;
delete label2;
delete selectModeButton;
delete dragModeButton;
delete openGlButton;
delete antialiasButton;
delete printButton;
delete resetButton;
delete zoomSlider;
delete rotateSlider;

}

QGraphicsView *Plot::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void Plot::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    //resetButton->setEnabled(false);
}

void Plot::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void Plot::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setMatrix(matrix);
   // setResetButtonEnabled();
}


void Plot::scale_me()
{
    qreal scale = graphicsView->actual_scale;
    if(scale>1.2)
        scale=1.2;
    if(scale<0.5)
        scale=0.5;
    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setMatrix(matrix);
    graphicsView->viewport()->update();
    //setResetButtonEnabled();
}


void Plot::togglePointerMode()
{
    graphicsView->setDragMode(selectModeButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    graphicsView->setInteractive(selectModeButton->isChecked());
}

void Plot::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}



void Plot::toggleAntialiasing()
{
    graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void Plot::print()
{

}


void Plot::do_zoomIn(){
     zoomSlider->setValue(zoomSlider->value() + 10);
     setupMatrix();
}

void Plot::do_zoomOut(){
     zoomSlider->setValue(zoomSlider->value() - 10);
     setupMatrix();
}


void Plot::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void Plot::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void Plot::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void Plot::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}

void Plot::resizeEvent(QResizeEvent *event)  {
    m_resizeTimer->setSingleShot(true);
    connect(m_resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()));
    m_resizeTimer->start(500);
    QWidget::resizeEvent(event);
}

void Plot:: resizeDone(){
    emit resized();
}
/////
