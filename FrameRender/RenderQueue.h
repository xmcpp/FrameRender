#ifndef _RENDERQUEUE_H_
#define _RENDERQUEUE_H_
/*
�൱����Ⱦ���ߣ����ݺ͸���״̬�����õ����������
*/

class RenderTarget;
class RenderInput;

class RenderQueue
{
public:
    //��������Ⱦ����
    RenderQueue( HWND hwnd , int width , int height , COLOR_TYPE type = R8G8B8A8 );
    ~RenderQueue();
public:
	void setRenderTarget( RenderTarget * target )
    {
        m_backTarget = target ;
    }

    RenderTarget * getRenderTarget(){ return m_backTarget; }

	void setWorldMat( const MATRIX4X4 & mat ){ m_worldMat = mat ;}
	void setViewMat( const MATRIX4X4 & mat ){ m_viewMat = mat ;}
	void setProjectMat( const MATRIX4X4 & mat ){ m_projectMat = mat ;}

	void drawRenderInput( RenderInput * input );
    
    //��ʼ�ͽ�����ǰ����
    void beginScene();
    void endScene();

    //�������浽��Ļ
    void present();
    
public: //���ò��ֵĺ���
    //�Ƿ�������������α߿�
    void enableDrawFrame( bool val ) { m_enableDrawFrame = val; }
    
    //���ñ���ü��ķ�ʽ
    void setCullingType( FACE_CULLING_TYPE type ){ m_cullingType = type; }
    
    //���ù�դ����䷽ʽ
    void setFillType( FILL_MODE type ){ m_fillType = type; }
private:
	//�ֲ���������ת��������
    void transVertexLocalToWorld( MATRIX4X4_PTR mat , RenderInput * input );
    
    //��������ת���������
    void transVertexWorldToView( MATRIX4X4_PTR mat , RenderInput * input );
    
    //���������תͶӰ����
    void transVertexViewToPrj( MATRIX4X4_PTR mat , RenderInput * input );
    
    //���ݱ任����任UV����
    void transUV( MATRIX4X4_PTR mat , RenderInput * input );

    //ͶӰ����ת��Ļ����
	void processScreenMap();
    
    //��ü�
    void faceCulling();

    //����դ��
    void rasterization();
    
    //���������
    void fillTriangle( FACE_TYPE_PTR pFace );
    void fillTopFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 ); //���ƽ��������
    void fillBottomFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 );//���ƽ��������

    //��������߿�
    void drawFrame( FACE_TYPE_PTR pFace ); 

    //��dda��������ֱ��
    void drawDDALine( float x1 , float y1 , float x2 , float y2 , ColorValue & val = ColorValue(1.0f,1.0f,1.0f) );
    
    /** ����ɨ���ߣ�֮����ʹ�ö�����Ϣ���ƣ�����Ϊ�ڻ���ɨ���ߵ�ʱ����Ҫ����ɫ��uv���в�ֵ
        �����ö�����Ϣ��Ϊ������Ϊ�Ժ�������չ��
        v0 �� v1 x������ȵģ����Ǹ���v0��v1��yֵ�����ˮƽ��һϵ������
    @param v0 xֵ��С�Ķ���
    @param v1 xֵ�ϴ�Ķ���
    */
    void drawScanLine( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 );

    //����ȾĿ����������������
    void setPixel( int x , int y , const ColorValue & val );
private:
	RenderTarget * m_backTarget; //�󻺳�
    RenderTarget * m_frontTarget; //ǰ����

	MATRIX4X4 m_worldMat;
	MATRIX4X4 m_viewMat;
	MATRIX4X4 m_projectMat;

	RenderInput * m_inputData;

private: //���ò���
    bool m_enableDrawFrame; //��¼�Ƿ�������Ʊ߿�
    FILL_MODE m_fillType; //��դ����䷽ʽ
    FACE_CULLING_TYPE m_cullingType; //��¼��ü���ʽ


    HWND m_mainHwnd;
};


#endif //_RENDERQUEUE_H_

