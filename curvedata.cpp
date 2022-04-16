#include "curvedata.h"
#include "signaldata.h"

const SignalData &CurveData::values() const
{
    return SignalData::instance();
}

SignalData &CurveData::values()
{
    return SignalData::instance();
}

QPointF CurveData::sample( size_t i ) const
{
    return SignalData::instance().value( i );
}

size_t CurveData::size() const
{
    return SignalData::instance().size();
}

QRectF CurveData::boundingRect() const
{
    return SignalData::instance().boundingRect();
}

/////

const SignalData2 &CurveData2::values() const
{
    return SignalData2::instance();
}

SignalData2 &CurveData2::values()
{
    return SignalData2::instance();
}

QPointF CurveData2::sample( size_t i ) const
{
    return SignalData2::instance().value( i );
}

size_t CurveData2::size() const
{
    return SignalData2::instance().size();
}

QRectF CurveData2::boundingRect() const
{
    return SignalData2::instance().boundingRect();
}


/////



const SignalData3 &CurveData3::values() const
{
    return SignalData3::instance();
}

SignalData3 &CurveData3::values()
{
    return SignalData3::instance();
}

QPointF CurveData3::sample( size_t i ) const
{
    return SignalData3::instance().value( i );
}

size_t CurveData3::size() const
{
    return SignalData3::instance().size();
}

QRectF CurveData3::boundingRect() const
{
    return SignalData3::instance().boundingRect();
}
