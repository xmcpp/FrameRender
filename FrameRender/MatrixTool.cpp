#include "stdafx.h"
#include "MatrixTool.h"

void MatrixTool::initMatrix3X3( MATRIX3X3_PTR mat )
{
	memset( mat , 0 , sizeof(MATRIX3X3) );
	mat->a_00 = 1.0f;
	mat->a_11 = 1.0f;
	mat->a_22 = 1.0f;
}

void MatrixTool::initMatrix4X4( MATRIX4X4_PTR mat )
{
	memset( mat , 0 , sizeof(MATRIX4X4) );
	mat->a_00 = 1.0f;
	mat->a_11 = 1.0f;
	mat->a_22 = 1.0f;
	mat->a_33 = 1.0f;
}

void MatrixTool::vector3MulMatrix4X4( MATRIX4X4_PTR mat , VECTOR3_PTR vin , VECTOR3_PTR vout )
{
	vout->x = vin->x * mat->a_00 + vin->y * mat->a_10 + vin->z  * mat->a_20 + mat->a_30;
	vout->y = vin->x * mat->a_01 + vin->y * mat->a_11 + vin->z  * mat->a_21 + mat->a_31;
	vout->z = vin->x * mat->a_02 + vin->y * mat->a_12 + vin->z  * mat->a_22 + mat->a_32;
}

void MatrixTool::vector4MulMatrix4X4( MATRIX4X4_PTR mat , Vector4 * vin , Vector4 * vout )
{
    vout->x = vin->x * mat->a_00 + vin->y * mat->a_10 + vin->z  * mat->a_20 + vin->w * mat->a_30;
    vout->y = vin->x * mat->a_01 + vin->y * mat->a_11 + vin->z  * mat->a_21 + vin->w * mat->a_31;
    vout->z = vin->x * mat->a_02 + vin->y * mat->a_12 + vin->z  * mat->a_22 + vin->w * mat->a_32;
    vout->w = vin->x * mat->a_03 + vin->y * mat->a_13 + vin->z  * mat->a_23 + vin->w * mat->a_33;
}

void MatrixTool::vMulMatrix4X4( MATRIX4X4_PTR mat , float * dIn , float * dOut , int count )
{
    float out[4];
    out[0] = *dIn * mat->a_00 + *( dIn + 1 ) * mat->a_10 + *( dIn + 2 )  * mat->a_20 ;
    out[1] = *dIn * mat->a_01 + *( dIn + 1 ) * mat->a_11 + *( dIn + 2 )  * mat->a_21 ;
    out[2] = *dIn * mat->a_02 + *( dIn + 1 ) * mat->a_12 + *( dIn + 2 )  * mat->a_22 ;
    
    if( count == 4 )
    {
        out[0] += *( dIn + 3 ) * mat->a_30;
        out[1] +=  *( dIn + 3 ) * mat->a_31;
        out[2] +=  *( dIn + 3 ) * mat->a_32;
        out[3] = *dIn * mat->a_03 + *( dIn + 1 ) * mat->a_13 + *( dIn + 2 ) * mat->a_23 + *( dIn + 3 ) * mat->a_33;    
    }
    memcpy( dOut , out , count * sizeof(float) );
}

void MatrixTool::vectorRotateX( Vector3 & v , float rad )
{
    float sinr = sin(rad);
    float cosr = cos( rad );
    float  y , z;
    y = v.y * cosr - v.z * sinr;
    z = v.y * sinr + v.z * cosr;
    v.y = y;
    v.z = z;
}

void MatrixTool::vectorRotateY( Vector3 & v , float rad )
{
    float sinr = sin(rad);
    float cosr = cos( rad );
    float  x , z;
    x = v.x * cosr + v.z * sinr;
    z = -v.x * sinr + v.z * cosr;
    v.x = x;
    v.z = z;
    v.normalize();
}

void MatrixTool::vectorRotateZ( Vector3 & v , float rad )
{
    float sinr = sin(rad);
    float cosr = cos( rad );
    float  x , y;
    x = v.x * cosr - v.y * sinr;
    y = v.y * sinr + v.z * cosr;
    
    v.x = x;
    v.y = y;
}

void MatrixTool::Matrix4X4Mul( MATRIX4X4_PTR mat , MATRIX4X4_PTR mat2 , MATRIX4X4_PTR ret )
{
    ret->a_00 = mat->a_00 * mat2->a_00 + mat->a_01 * mat2->a_10 + mat->a_02 * mat2->a_20 + mat->a_03 * mat2->a_30;

}