#ifndef _RenderSystemGDI_H__
#define _RenderSystemGDI_H__

#include "RenderSystem.h"

class RenderSystemGDI : public RenderSystem
{
public:
	RenderSystemGDI(HWND hwnd );
	virtual ~RenderSystemGDI();
public:
	virtual void createRenderSystem( int width , int height , COLOR_TYPE type = R8G8B8A8 );
	virtual unsigned char * lockBuffer( int & pitch);
	virtual void unLockBuffer();
	virtual void beginScene();
	virtual void endScene();
	virtual void present();

private:
	unsigned char * m_frontBuff;
	unsigned char * m_backBuff;

	HWND m_hwnd;
};

#endif //_RenderSystemGDI_H__
