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
    //��ʼ��������
	void init( HWND hwnd , int width , int height );
    void setCamera( Camera * cam ){ m_cam = cam; }
    Camera * getCamera() { return m_cam; }
    //���һ����Ⱦ������ʱû�г������������������ݵĴ洢
	void addRenderObject( RenderObject * pObj );
    
    //��Ⱦ
    void renderOneFrame();
private:
	RenderQueue * m_renderQueue;
	Camera * m_cam;
	std::vector<RenderObject*> m_renderObjVec;
};

#endif //_RENDERROOT_

