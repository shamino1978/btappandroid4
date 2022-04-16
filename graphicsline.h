#ifndef GRAPHICSLINE_H
#define GRAPHICSLINE_H
#include <QGraphicsLineItem>
#include <QRect>
#include <QGraphicsItem>


class GraphicsLineItem : public QGraphicsLineItem
{

public:

     GraphicsLineItem ( QPen &pen, qreal x1,qreal x2,qreal y1,qreal y2, QGraphicsItem* parent = nullptr);

   // void  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
 //GraphicsLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr) ;
};

#endif // GRAPHICSLINE_H
