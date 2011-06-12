#ifndef _VECTOR3_H_
#define _VECTOR3_H_

class Vector3
{
public:
    Vector3()
        :x(0.0f),y(0.0f),z(0.0f) {}
    
    Vector3( float x , float y , float z )
        :x(x),y(y),z(z){}
    
    Vector3( const Vector3 & a )
        :x(a.x),y(a.y),z(a.z) {}
    
public:
    Vector3 operator + ( const Vector3 & a );
    Vector3 operator - ( const Vector3 & a );
    void operator += ( const Vector3 & a );
    Vector3 operator * ( float v );

    Vector3 dotProduct( const Vector3 & a );
    Vector3 crossProduct( const Vector3 & a );
    void normalize();
    
    //ÉùÃ÷¾²Ì¬³£Á¿
    static const Vector3 ZERO ;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
    static const Vector3 NEGATIVE_UNIT_X;
    static const Vector3 NEGATIVE_UNIT_Y;
    static const Vector3 NEGATIVE_UNIT_Z;
public:
    float x;
    float y;
    float z;
};

#endif //_VECTOR3_H_