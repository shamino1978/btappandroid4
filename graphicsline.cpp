#include "graphicsline.h"


GraphicsLineItem::GraphicsLineItem( QPen &pen, qreal x1,qreal x2,qreal y1,qreal y2, QGraphicsItem* parent):
    QGraphicsLineItem( parent)
{
   QGraphicsLineItem::setPen( pen);
    QGraphicsLineItem:: setLine(x1,y1,  x2,  y2);
}


