#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

class RenderTarget
{
public:
	RenderTarget();
    ~RenderTarget();
public:
	void createRenderTarget( int width , int height , COLOR_TYPE type = R8G8B8A8 );
	unsigned char * lockBuffer( int & pitch);
    int getBufferWidth(){ return m_width; }
    int getBufferHeight() { return m_height; }
    COLOR_TYPE getBufferColorType() { return m_colorType; }
    void reset();
private:
	unsigned char * m_buff;
	int m_width;
	int m_height;
	
	int m_pitch; //¼ÇÂ¼¿ç¶ÈÖµ
	COLOR_TYPE m_colorType;
};

#endif //_RENDERTARGET_H_
