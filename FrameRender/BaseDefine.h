#ifndef _BASEDEFINE_H_
#define  _BASEDEFINE_H_

enum COLOR_TYPE{ R8G8B8A8 = 4 };

//面裁减方式
enum FACE_CULLING_TYPE{ NONE = 1 , //不裁减
                        CW = 2 , //裁减顺时针的面
                        CCW = 3  //裁减逆时针的面
                      };

//填充方式
enum FILL_MODE{ FILL_POINT = 1 ,
                FILL_WIREFRAME = 2 ,
                FILL_SOLIDE = 3 };


//顶点类型
typedef struct VertexType
{
	float x,y,z,w; //记录变换前顶点
	float nx,ny,nz; //法线
	float u,v; //uv坐标
    float r,g,b,a; //顶点颜色分量
}VERTEX_TYPE,*VERTEX_TYPE_PTR;

//变换后的顶点类型
typedef struct VertexTypeTransed
{
	int state; //记录顶点状态 0:正常，1:裁减
    int sx , sy; //映射到屏幕的坐标
	float x,y,z,w;
	float nx,ny,nz,nw;
	float u,v;
    float r,g,b,a;//顶点颜色分量
}VERTEX_TYPE_TRANSED,*VERTEX_TYPE_TRANSED_PTR;

//面类型
typedef struct FaceType
{
    int state; //记录面的状态 0:正常，1:裁减
    long i1,i2,i3;//记录该面的3个顶点索引

}FACE_TYPE ,*FACE_TYPE_PTR;

//浮点数比较
const float   FLT_EPSILON  = 1.192092896e-07F;
#define F_CMP( a, b ) ( fabs( a - b ) < FLT_EPSILON ? 1:0 )

const float FR_PI = 3.1415926535f;

#endif //_BASEDEFINE_H_