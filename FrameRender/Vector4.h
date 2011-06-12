#ifndef _VECTOR4_H_
#define _VECTOR4_H_

class Vector4
{
public:
    Vector4()
        :x(0.0f),y(0.0f),z(0.0f),w(1.0f) {}

    Vector4( float x , float y , float z ,float w = 1.0f )
        :x(x),y(y),z(z),w(w){}

    Vector4( const Vector4 & a )
        :x(a.x),y(a.y),z(a.z),w(a.w) {}
    
    Vector4( const Vector3 & a )
        :x(a.x),y(a.y),z(a.z),w(1.0f) {}
public:
    Vector4 operator + ( const Vector4 & a );
    Vector4 operator - ( const Vector4 & a );
    
    Vector4 dotProduct( const Vector4 & a );
    void setValue( float x , float y , float z , float w = 1.0f );

public:
    float x;
    float y;
    float z;
    float w;
};

#endif //_VECTOR4_H_