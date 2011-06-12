#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget( )
:m_pitch(0)
{

}

RenderTarget::~RenderTarget()
{
    delete []m_buff;
}

void RenderTarget::createRenderTarget( int width , int height , COLOR_TYPE type )
{
	m_width = width;
	m_height = height;
	m_colorType = type;

	m_buff = new unsigned char[ width * height * type ];
	m_pitch = width * type;
}

unsigned char * RenderTarget::lockBuffer( int & pitch )
{
	pitch = m_pitch;
	return m_buff;
}

void RenderTarget::reset()
{
    memset( m_buff , 0 , m_width * m_height * m_colorType );
}