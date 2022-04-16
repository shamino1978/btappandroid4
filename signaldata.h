#ifndef SIGNALDATA_H
#define SIGNALDATA_H


#include <qrect.h>

class SignalData
{
public:
    static SignalData &instance();

    void append( const QPointF &pos );
    void clearStaleValues( double min );

    int size() const;
    QPointF value( int index ) const;

    QRectF boundingRect() const;

    void lock();
    void unlock();


private:
    SignalData();
    SignalData( const SignalData & );
    SignalData &operator=( const SignalData & );

    virtual ~SignalData();

    class PrivateData;
    PrivateData *d_data;

};



class SignalData2
{
public:
    static SignalData2 &instance();

    void append( const QPointF &pos );
    void clearStaleValues( double min );

    int size() const;
    QPointF value( int index ) const;

    QRectF boundingRect() const;

    void lock();
    void unlock();


private:
    SignalData2();
    SignalData2( const SignalData2 & );
    SignalData2 &operator=( const SignalData2 & );

    virtual ~SignalData2();

    class PrivateData;
    PrivateData *d_data;

};




////



class SignalData3
{
public:
    static SignalData3 &instance();

    void append( const QPointF &pos );
    void clearStaleValues( double min );

    int size() const;
    QPointF value( int index ) const;

    QRectF boundingRect() const;

    void lock();
    void unlock();


private:
    SignalData3();
    SignalData3( const SignalData3 & );
    SignalData3 &operator=( const SignalData3 & );

    virtual ~SignalData3();

    class PrivateData;
    PrivateData *d_data;

};


#endif // SIGNALDATA_H
