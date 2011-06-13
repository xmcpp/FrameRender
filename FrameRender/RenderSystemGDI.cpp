#include "stdafx.h"
#include "RenderSystemGDI.h"

RenderSystemGDI::RenderSystemGDI( HWND hwnd )
:m_backBuff(NULL),m_frontBuff(NULL),m_hwnd( hwnd )
{

}

RenderSystemGDI::~RenderSystemGDI()
{
	delete []m_frontBuff;
	delete []m_backBuff;
}

void RenderSystemGDI::createRenderSystem( int width , int height , COLOR_TYPE type )
{
	m_width = width;
	m_height = height;
	m_colorType = type;

	m_frontBuff = new unsigned char[ width * height * type ];
	m_backBuff = new unsigned char[ width * height * type ];
	m_pitch = width * type;
}

unsigned char * RenderSystemGDI::lockBuffer( int & pitch)
{
	pitch = m_pitch;
	return m_backBuff;
}

void RenderSystemGDI::unLockBuffer()
{

}

void RenderSystemGDI::beginScene()
{
	memset( m_backBuff , 0 , m_width * m_height * m_colorType );
}

void RenderSystemGDI::endScene()
{

}

void RenderSystemGDI::present()
{
	//���󻺳������ύ
	HDC hdc;
	hdc = GetDC(m_hwnd);

	for( int i = 0 ; i < m_width ; i++ )
	{
		for( int j = 0 ; j < m_height ; j++ )
		{
			int tmp = j*m_pitch + i*m_colorType;
			//��Ϊ��ɫRGBA���д洢������R�ڸ�λ��a�ڵ�λ������ȡֵʱҪע�ⷴ����
			SetPixel( hdc , i , j , RGB(m_backBuff[tmp+3] , m_backBuff[tmp+2] , m_backBuff[tmp+1]) );
		}
	}
	ReleaseDC( m_hwnd , hdc );

	//����������
	unsigned char * tmp = m_backBuff;
	m_backBuff = m_frontBuff;
	m_frontBuff = tmp;

	Sleep(10);
}

