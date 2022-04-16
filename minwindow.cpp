#include "mainwindow.h"


mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_mainwindow)
{
    //! [Construct UI]
    ui->setupUi(this);

}

mainwindow::~mainwindow()
{

}
