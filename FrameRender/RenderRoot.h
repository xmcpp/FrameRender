#ifndef _RENDERROOT_
#define _RENDERROOT_

class RenderSystem;
class RenderQueue;
class RenderObject;
class Camera;

class RenderRoot
{
public:
    RenderRoot();
    ~RenderRoot();
public:
    //初始化根对象
	void init( HWND hwnd , int width , int height );
    void setCamera( Camera * cam ){ m_cam = cam; }
    Camera * getCamera() { return m_cam; }
    //添加一个渲染对象，暂时没有场景管理器来负责数据的存储
	void addRenderObject( RenderObject * pObj );
    
    //渲染
    void renderOneFrame();
private:
	RenderQueue * m_renderQueue;
	Camera * m_cam;
	std::vector<RenderObject*> m_renderObjVec;
};

#endif //_RENDERROOT_

