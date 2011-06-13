#ifndef _RenderSystem_H_
#define _RenderSystem_H_

class RenderSystem
{
public:
	RenderSystem();
	virtual ~RenderSystem(){}

public:
	int getBufferWidth(){ return m_width; }
	int getBufferHeight() { return m_height; }
	COLOR_TYPE getBufferColorType() { return m_colorType; }

public:
	virtual void createRenderSystem( int width , int height , COLOR_TYPE type = R8G8B8A8 ) = 0;
	virtual unsigned char * lockBuffer( int & pitch) = 0;
	virtual void unLockBuffer() = 0;
    virtual void beginScene() = 0;
	virtual void endScene() = 0;
	virtual void present() = 0;

protected:
	int m_width;
	int m_height;
	int m_pitch; //¼ÇÂ¼¿ç¶ÈÖµ
	COLOR_TYPE m_colorType;
};

#endif //_RenderSystem_H_
