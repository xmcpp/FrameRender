#ifndef _COLORVALUE_H_
#define _COLORVALUE_H_

class ColorValue
{
public:
    ColorValue( float r , float g , float b , float a = 1.0f )
        :r(r),g(g),b(b),a(a){}
    
public:
    void setR( float rVal ){ r = rVal; }
    void setG( float gVal ){ g = gVal; }
    void setB( float bVal ){ b = bVal; }
    void setA( float aVal ){ a = aVal; }
    void setRGB( float rVal , float gVal , float bVal ){ r = rVal ; g = gVal ; b = bVal; }
    void saturate();

    //按rgba的方式获得颜色值
    int getARGB() ;
	unsigned char getR();
	unsigned char getG();
	unsigned char getB();
	unsigned char getA();

private:
    float r;
    float g;
    float b;
    float a;
};

#endif //_COLORVALUE_H_
