#ifndef _RENDERQUEUE_H_
#define _RENDERQUEUE_H_
/*
相当于渲染管线，数据和各种状态都设置到这个对象中
*/

class RenderTarget;
class RenderInput;

class RenderQueue
{
public:
    //构造主渲染窗口
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
    
    //开始和结束当前场景
    void beginScene();
    void endScene();

    //交换缓存到屏幕
    void present();
    
public: //配置部分的函数
    //是否允许绘制三角形边框
    void enableDrawFrame( bool val ) { m_enableDrawFrame = val; }
    
    //设置背面裁减的方式
    void setCullingType( FACE_CULLING_TYPE type ){ m_cullingType = type; }
    
    //设置光栅器填充方式
    void setFillType( FILL_MODE type ){ m_fillType = type; }
private:
	//局部顶点坐标转世界坐标
    void transVertexLocalToWorld( MATRIX4X4_PTR mat , RenderInput * input );
    
    //世界坐标转摄像机坐标
    void transVertexWorldToView( MATRIX4X4_PTR mat , RenderInput * input );
    
    //摄像机坐标转投影坐标
    void transVertexViewToPrj( MATRIX4X4_PTR mat , RenderInput * input );
    
    //根据变换矩阵变换UV坐标
    void transUV( MATRIX4X4_PTR mat , RenderInput * input );

    //投影坐标转屏幕坐标
	void processScreenMap();
    
    //面裁减
    void faceCulling();

    //处理栅格化
    void rasterization();
    
    //三角形填充
    void fillTriangle( FACE_TYPE_PTR pFace );
    void fillTopFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 ); //填充平顶三角形
    void fillBottomFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 );//填充平底三角形

    //给面绘制线框
    void drawFrame( FACE_TYPE_PTR pFace ); 

    //用dda方法绘制直线
    void drawDDALine( float x1 , float y1 , float x2 , float y2 , ColorValue & val = ColorValue(1.0f,1.0f,1.0f) );
    
    /** 绘制扫描线，之所以使用顶点信息绘制，是因为在绘制扫描线的时候还需要对颜色和uv进行插值
        所以用顶点信息作为参数，为以后留下扩展性
        v0 和 v1 x轴是相等的，就是根据v0和v1的y值来填充水平的一系列像素
    @param v0 x值较小的顶点
    @param v1 x值较大的顶点
    */
    void drawScanLine( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 );

    //在渲染目标上设置像素内容
    void setPixel( int x , int y , const ColorValue & val );
private:
	RenderTarget * m_backTarget; //后缓冲
    RenderTarget * m_frontTarget; //前缓冲

	MATRIX4X4 m_worldMat;
	MATRIX4X4 m_viewMat;
	MATRIX4X4 m_projectMat;

	RenderInput * m_inputData;

private: //配置部分
    bool m_enableDrawFrame; //记录是否允许绘制边框
    FILL_MODE m_fillType; //光栅器填充方式
    FACE_CULLING_TYPE m_cullingType; //记录面裁减方式


    HWND m_mainHwnd;
};


#endif //_RENDERQUEUE_H_

