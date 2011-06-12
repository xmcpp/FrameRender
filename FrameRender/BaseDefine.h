#ifndef _BASEDEFINE_H_
#define  _BASEDEFINE_H_

enum COLOR_TYPE{ R8G8B8A8 = 4 };

//��ü���ʽ
enum FACE_CULLING_TYPE{ NONE = 1 , //���ü�
                        CW = 2 , //�ü�˳ʱ�����
                        CCW = 3  //�ü���ʱ�����
                      };

//��䷽ʽ
enum FILL_MODE{ FILL_POINT = 1 ,
                FILL_WIREFRAME = 2 ,
                FILL_SOLIDE = 3 };


//��������
typedef struct VertexType
{
	float x,y,z,w; //��¼�任ǰ����
	float nx,ny,nz; //����
	float u,v; //uv����
    float r,g,b,a; //������ɫ����
}VERTEX_TYPE,*VERTEX_TYPE_PTR;

//�任��Ķ�������
typedef struct VertexTypeTransed
{
	int state; //��¼����״̬ 0:������1:�ü�
    int sx , sy; //ӳ�䵽��Ļ������
	float x,y,z,w;
	float nx,ny,nz,nw;
	float u,v;
    float r,g,b,a;//������ɫ����
}VERTEX_TYPE_TRANSED,*VERTEX_TYPE_TRANSED_PTR;

//������
typedef struct FaceType
{
    int state; //��¼���״̬ 0:������1:�ü�
    long i1,i2,i3;//��¼�����3����������

}FACE_TYPE ,*FACE_TYPE_PTR;

//�������Ƚ�
const float   FLT_EPSILON  = 1.192092896e-07F;
#define F_CMP( a, b ) ( fabs( a - b ) < FLT_EPSILON ? 1:0 )

const float FR_PI = 3.1415926535f;

#endif //_BASEDEFINE_H_