#ifndef CURVEDATA_H
#define CURVEDATA_H


//#include <qwt_series_data.h>
#include <qpointer.h>

class SignalData;
class SignalData2;
class SignalData3;

class CurveData: public QwtSeriesData<QPointF>
{
public:
    const SignalData &values() const;
    SignalData &values();

    virtual QPointF sample( size_t i ) const;
    virtual size_t size() const;

    virtual QRectF boundingRect() const;

};


class CurveData2: public QwtSeriesData<QPointF>
{
public:
    const SignalData2 &values() const;
    SignalData2 &values();

    virtual QPointF sample( size_t i ) const;
    virtual size_t size() const;

    virtual QRectF boundingRect() const;

};

class CurveData3: public QwtSeriesData<QPointF>
{
public:
    const SignalData3 &values() const;
    SignalData3 &values();

    virtual QPointF sample( size_t i ) const;
    virtual size_t size() const;

    virtual QRectF boundingRect() const;

};



#endif // CURVEDATA_H
