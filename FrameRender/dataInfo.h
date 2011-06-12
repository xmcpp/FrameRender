#ifndef _DATAINFO_H_
#define _DATAINFO_H_

typedef struct VECTOR3_TYPE
{
	float x;
	float y;
	float z;
}VECTOR3 , *VECTOR3_PTR;

typedef struct POINT4D_TYPE
{
    float x;
    float y;
    float z;
    float w;
}POINT4D , * POINT4D_PTR;

typedef struct MATRIX4X4_TYPE
{
	float a_00;
	float a_01;
	float a_02;
	float a_03;

	float a_10;
	float a_11;
	float a_12;
	float a_13;

	float a_20;
	float a_21;
	float a_22;
	float a_23;

	float a_30;
	float a_31;
	float a_32;
	float a_33;
} MATRIX4X4 , *MATRIX4X4_PTR;

typedef struct MATRIX3X3_TYPE
{
	float a_00;
	float a_01;
	float a_02;

	float a_10;
	float a_11;
	float a_12;

	float a_20;
	float a_21;
	float a_22;

} MATRIX3X3 , *MATRIX3X3_PTR;

#endif //_DATAINFO_H_
