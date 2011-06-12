#ifndef _MATRIXTOOL_H_
#define _MATRIXTOOL_H_

class MatrixTool
{
public:
	static void initMatrix3X3( MATRIX3X3_PTR mat );
	static void initMatrix4X4( MATRIX4X4_PTR mat );
	
	static void vector3MulMatrix4X4( MATRIX4X4_PTR mat , VECTOR3_PTR vin , VECTOR3_PTR vout );
    
    static void vector4MulMatrix4X4( MATRIX4X4_PTR mat , Vector4 * vin , Vector4 * vout );
    
    //向量与矩阵相乘
    static void vMulMatrix4X4( MATRIX4X4_PTR mat , float * dIn , float * dOut , int count );

    //向量绕x轴旋转 rad为弧度
    static void vectorRotateX( Vector3 & v , float rad );

    //向量绕y轴旋转 rad为弧度
    static void vectorRotateY( Vector3 & v , float rad );

    //向量绕z轴旋转 rad为弧度
    static void vectorRotateZ( Vector3 & v , float rad );
    
    //4X4矩阵相乘
    static void Matrix4X4Mul( MATRIX4X4_PTR mat , MATRIX4X4_PTR mat2 , MATRIX4X4_PTR ret );
};

#endif //_MATRIXTOOL_H_
