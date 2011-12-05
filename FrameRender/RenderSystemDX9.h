#ifndef __RENDERSYSTEMDX9_H__
#define __RENDERSYSTEMDX9_H__

#include "RenderSystem.h"

class RenderSystemDX9 : public RenderSystem
{
public:
	RenderSystemDX9(HWND hwnd );
	virtual ~RenderSystemDX9();
public:
	struct CUSTOMTEXTUREVERTEX
	{
		float x , y , z ,w; //xyz-rhw
		DWORD color;
		float ux , uy;		//uv
		
	};
public:
	virtual void createRenderSystem( int width , int height , COLOR_TYPE type = R8G8B8A8 );
	virtual unsigned char * lockBuffer( int & pitch);
	virtual void unLockBuffer();
	virtual void beginScene();
	virtual void endScene();
	virtual void present();

private:
	bool initD3d();
private:
	HWND m_hwnd;

	//D3D设备指针
	IDirect3DDevice9 * m_p3d9Dev ;
	IDirect3D9 * m_d3d ;
	
	IDirect3DVertexBuffer9 * m_vertexBuff;
	IDirect3DIndexBuffer9 * m_indexBuff;
	//贴图和锁定的数据
	IDirect3DTexture9 * m_texture;


	bool m_isValid;
};

#endif //__RENDERSYSTEMDX9_H__
