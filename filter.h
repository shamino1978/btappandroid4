#ifndef FILTER_H
#define FILTER_H


#include <QWidget>

namespace Ui {
class filter;
}

class filter : public QWidget
{
    Q_OBJECT

public:
    explicit filter(QWidget *parent = nullptr);

    ~filter();


public slots:


private slots:


    void on_pushButton_clicked();

private:
    Ui::filter *ui;

};

#endif // FILTER_H
