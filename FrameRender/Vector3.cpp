#include "stdafx.h"
#include "Vector3.h"

const Vector3 Vector3::ZERO( 0.0f , 0.0f , 0.0f );
const Vector3 Vector3::UNIT_X( 1, 0, 0 );
const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );

Vector3 Vector3::operator + ( const Vector3 & a )
{
    return Vector3( x + a.x , y + a.y , z + a.z );
}

Vector3 Vector3::operator - ( const Vector3 & a )
{
    return Vector3( x - a.x , y - a.y , z- a.z );
}

void Vector3::operator += ( const Vector3 & a )
{
    x = x + a.x ;
    y = y + a.y ;
    z = z + a.z ;
}

Vector3 Vector3::operator * ( float v )
{
    x = x * v;
    y = y * v;
    z = z * v;
    return *this;
}

Vector3 Vector3::dotProduct( const Vector3 & a )
{
    return Vector3( x * a.x , y * a.y , z * a.z );
}

Vector3 Vector3::crossProduct( const Vector3 & a )
{
    Vector3 vec;
    vec.x = y * a.z - z * a.y;
    vec.y = z * a.x - x * a.z;
    vec.z = x * a.y - y * a.x;
    return vec;
}

void Vector3::normalize()
{
    float tmp = x*x + y*y + z*z ;
    if( tmp > 0.0f  )
    {
        tmp = 1.0f / sqrt( tmp );
        x *= tmp;
        y *= tmp;
        z *= tmp;
    }
}
