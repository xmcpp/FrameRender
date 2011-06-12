#include "stdafx.h"
#include "ColorValue.h"

int ColorValue::getRGBA()
{
    int value = (int)( r * 255 ) << 24;
    value = value | (int)( g * 255 ) << 16;
    value = value | (int)( b * 255 ) << 8;
    value = value | (int)( a * 255 );

    return value;
}

void ColorValue::saturate()
{
    if (r < 0)
        r = 0;
    else if (r > 1)
        r = 1;

    if (g < 0)
        g = 0;
    else if (g > 1)
        g = 1;

    if (b < 0)
        b = 0;
    else if (b > 1)
        b = 1;

    if (a < 0)
        a = 0;
    else if (a > 1)
        a = 1;
}