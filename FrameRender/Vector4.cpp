#include "stdafx.h"
#include "Vector4.h"

Vector4 Vector4::operator + ( const Vector4 & a )
{
    return Vector4( x + a.x , y + a.y , z + a.z );
}

Vector4 Vector4::operator - ( const Vector4 & a )
{
    return Vector4( x - a.x , y - a.y , z- a.z );
}

Vector4 Vector4::dotProduct( const Vector4 & a )
{
    return Vector4( x * a.x , y * a.y , z * a.z , w * a.w );
}

void Vector4::setValue( float x , float y , float z , float w )
{
    x = x;y = y;z = z;w = w;
}



