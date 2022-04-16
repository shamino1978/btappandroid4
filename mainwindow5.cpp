#include "mainwindow5.h"
#include <QHBoxLayout>


mainwindow5::mainwindow5(vector<vector<double>> &qvec, QWidget *parent):
   QWidget(parent)
{

  this->setWindowTitle("Playback");
    d_plot=new Plot( this );
   // d_plot->draw_vect(qvec);
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );

}


mainwindow5::mainwindow5(vector<vector<double>> &qvec,vector<vector<double>> &qvec2,  QWidget *parent):
   QWidget(parent)
{

  this->setWindowTitle("Compare");
    d_plot=new Plot( this );
   // d_plot->draw_vect(qvec,qvec2);
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );

}

mainwindow5:: ~mainwindow5(){
    delete d_plot;
}
