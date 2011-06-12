#ifndef _MATRIXTOOL_H_
#define _MATRIXTOOL_H_

class MatrixTool
{
public:
	static void initMatrix3X3( MATRIX3X3_PTR mat );
	static void initMatrix4X4( MATRIX4X4_PTR mat );
	
	static void vector3MulMatrix4X4( MATRIX4X4_PTR mat , VECTOR3_PTR vin , VECTOR3_PTR vout );
    
    static void vector4MulMatrix4X4( MATRIX4X4_PTR mat , Vector4 * vin , Vector4 * vout );
    
    //������������
    static void vMulMatrix4X4( MATRIX4X4_PTR mat , float * dIn , float * dOut , int count );

    //������x����ת radΪ����
    static void vectorRotateX( Vector3 & v , float rad );

    //������y����ת radΪ����
    static void vectorRotateY( Vector3 & v , float rad );

    //������z����ת radΪ����
    static void vectorRotateZ( Vector3 & v , float rad );
    
    //4X4�������
    static void Matrix4X4Mul( MATRIX4X4_PTR mat , MATRIX4X4_PTR mat2 , MATRIX4X4_PTR ret );
};

#endif //_MATRIXTOOL_H_
