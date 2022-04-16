/******************************************************************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_CURVE_H
#define QWT_PLOT_CURVE_H

#include "qwt_global.h"
#include "qwt_plot_seriesitem.h"

#include <qstring.h>

class QwtScaleMap;
class QwtSymbol;
class QwtCurveFitter;
template< typename T > class QwtSeriesData;
class QwtText;
class QPainter;
class QPolygonF;
class QPen;

/*!
   \brief A plot item, that represents a series of points

   A curve is the representation of a series of points in the x-y plane.
   It supports different display styles, interpolation ( f.e. spline )
   and symbols.

   \par Usage
   <dl><dt>a) Assign curve properties</dt>
   <dd>When a curve is created, it is configured to draw black solid lines
   with in QwtPlotCurve::Lines style and no symbols.
   You can change this by calling
   setPen(), setStyle() and setSymbol().</dd>
   <dt>b) Connect/Assign data.</dt>
   <dd>QwtPlotCurve gets its points using a QwtSeriesData object offering
   a bridge to the real storage of the points ( like QAbstractItemModel ).
   There are several convenience classes derived from QwtSeriesData, that also store
   the points inside ( like QStandardItemModel ). QwtPlotCurve also offers
   a couple of variations of setSamples(), that build QwtSeriesData objects from
   arrays internally.</dd>
   <dt>c) Attach the curve to a plot</dt>
   <dd>See QwtPlotItem::attach()
   </dd></dl>

   \par Example:
   see examples/bode

   \sa QwtPointSeriesData, QwtSymbol, QwtScaleMap
 */
class QwtPlotCurve2
    : public QwtPlotSeriesItem
    , public QwtSeriesStore< QPointF >
{
  public:

    enum CurveStyle
    {

        NoCurve = -1,

        Lines,


        Sticks,

        Steps,

        Dots,

        UserCurve = 100
    };

    enum CurveAttribute
    {

        Inverted = 0x01,

        Fitted = 0x02
    };

    Q_DECLARE_FLAGS( CurveAttributes, CurveAttribute )

    enum LegendAttribute
    {

        LegendNoAttribute = 0x00,

        LegendShowLine = 0x01,

        LegendShowSymbol = 0x02,

        LegendShowBrush = 0x04
    };

    Q_DECLARE_FLAGS( LegendAttributes, LegendAttribute )

    enum PaintAttribute
    {

        ClipPolygons = 0x01,

        FilterPoints = 0x02,

        MinimizeMemory = 0x04,

        ImageBuffer = 0x08,

        FilterPointsAggressive = 0x10,
    };

    Q_DECLARE_FLAGS( PaintAttributes, PaintAttribute )

    explicit QwtPlotCurve( const QString& title = QString() );
    explicit QwtPlotCurve( const QwtText& title );

    virtual ~QwtPlotCurve();

    virtual int rtti() const QWT_OVERRIDE;

    void setPaintAttribute( PaintAttribute, bool on = true );
    bool testPaintAttribute( PaintAttribute ) const;

    void setLegendAttribute( LegendAttribute, bool on = true );
    bool testLegendAttribute( LegendAttribute ) const;

    void setLegendAttributes( LegendAttributes );
    LegendAttributes legendAttributes() const;

    void setRawSamples( const double* xData, const double* yData, int size );
    void setRawSamples( const float* xData, const float* yData, int size );

    void setRawSamples( const double* yData, int size );
    void setRawSamples( const float* yData, int size );

    void setSamples( const double* xData, const double* yData, int size );
    void setSamples( const float* xData, const float* yData, int size );

    void setSamples( const double* yData, int size );
    void setSamples( const float* yData, int size );

    void setSamples( const QVector< double >& yData );
    void setSamples( const QVector< float >& yData );

    void setSamples( const QVector< double >& xData, const QVector< double >& yData );
    void setSamples( const QVector< float >& xData, const QVector< float >& yData );

    void setSamples( const QVector< QPointF >& );
    void setSamples( QwtSeriesData< QPointF >* );

    virtual int closestPoint( const QPointF& pos, double* dist = NULL ) const;

    double minXValue() const;
    double maxXValue() const;
    double minYValue() const;
    double maxYValue() const;

    void setCurveAttribute( CurveAttribute, bool on = true );
    bool testCurveAttribute( CurveAttribute ) const;

    void setPen( const QColor&, qreal width = 0.0, Qt::PenStyle = Qt::SolidLine );
    void setPen( const QPen& );
    const QPen& pen() const;

    void setBrush( const QBrush& );
    const QBrush& brush() const;

    void setBaseline( double );
    double baseline() const;

    void setStyle( CurveStyle style );
    CurveStyle style() const;

    void setSymbol( QwtSymbol* );
    const QwtSymbol* symbol() const;

    void setCurveFitter( QwtCurveFitter* );
    QwtCurveFitter* curveFitter() const;

    virtual void drawSeries( QPainter*,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const QWT_OVERRIDE;

    virtual QwtGraphic legendIcon( int index, const QSizeF& ) const QWT_OVERRIDE;

  protected:

    void init();

    virtual void drawCurve( QPainter*, int style,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void drawSymbols( QPainter*, const QwtSymbol&,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void drawLines( QPainter*,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void drawSticks( QPainter*,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void drawDots( QPainter*,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void drawSteps( QPainter*,
        const QwtScaleMap& xMap, const QwtScaleMap& yMap,
        const QRectF& canvasRect, int from, int to ) const;

    virtual void fillCurve( QPainter*,
        const QwtScaleMap&, const QwtScaleMap&,
        const QRectF& canvasRect, QPolygonF& ) const;

    void closePolyline( QPainter*,
        const QwtScaleMap&, const QwtScaleMap&, QPolygonF& ) const;

  private:
    class PrivateData;
    PrivateData* m_data;
};

//! boundingRect().left()
inline double QwtPlotCurve::minXValue() const
{
    return boundingRect().left();
}

//! boundingRect().right()
inline double QwtPlotCurve::maxXValue() const
{
    return boundingRect().right();
}

//! boundingRect().top()
inline double QwtPlotCurve::minYValue() const
{
    return boundingRect().top();
}

//! boundingRect().bottom()
inline double QwtPlotCurve::maxYValue() const
{
    return boundingRect().bottom();
}

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotCurve::PaintAttributes )
Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotCurve::LegendAttributes )
Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotCurve::CurveAttributes )

#endif
