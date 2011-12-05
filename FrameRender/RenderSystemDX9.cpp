#include "stdafx.h"
#include "RenderSystemDX9.h"

RenderSystemDX9::RenderSystemDX9(HWND hwnd )
:m_isValid(false),m_hwnd(hwnd)
{

}

RenderSystemDX9::~RenderSystemDX9()
{

}

void RenderSystemDX9::createRenderSystem( int width , int height , COLOR_TYPE type )
{
	m_width = width;
	m_height = height;

	CUSTOMTEXTUREVERTEX data[4] = 
	{
		{ 0.0f , 0.0f , 0.5f , 1.0f  , 0xffff0000 ,1,0},
		{ 256.0f , 0.0f , 0.5f , 1.0f  , 0xffff0000 ,1,1},
		{ 0.0f , 256.0f , 0.5f , 1.0f  , 0xffff0000 ,0,0},
		{ 256.0f , 256.0f , 0.5f , 1.0f  , 0xffff0000 ,0,1}
	};
	//初始化d3d设备
	m_isValid = initD3d();
	
	//创建贴图
	HRESULT hr = m_p3d9Dev->CreateTexture( width , height , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , &m_texture , NULL );

	//创建顶点缓存
	m_p3d9Dev->CreateVertexBuffer( 4 * sizeof(CUSTOMTEXTUREVERTEX) , 0 , D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 , D3DPOOL_DEFAULT , &m_vertexBuff , NULL );
	
	float * pData = NULL;
	m_vertexBuff->Lock( 0 , 4 * sizeof(CUSTOMTEXTUREVERTEX) , (void**)&pData , NULL );
	memcpy( pData , data , 4 * sizeof(CUSTOMTEXTUREVERTEX) );
	m_vertexBuff->Unlock();

	//创建索引数据
	WORD * pIndexData = NULL;
	m_p3d9Dev->CreateIndexBuffer( 6*sizeof(WORD) , 0 , D3DFMT_INDEX16 ,D3DPOOL_DEFAULT , &m_indexBuff , NULL );
	m_indexBuff->Lock( 0 , 6*sizeof(WORD) , (void**)&pIndexData , NULL );
	*(pIndexData++) = 0;
	*(pIndexData++) = 1;
	*(pIndexData++) = 2;

	*(pIndexData++) = 1;
	*(pIndexData++) = 3;
	*(pIndexData++) = 2;
	m_indexBuff->Unlock();
	

	//unLockBuffer();
}

unsigned char * RenderSystemDX9::lockBuffer( int & pitch)
{
	D3DLOCKED_RECT rect;
	HRESULT hr = m_texture->LockRect( 0 , &rect , NULL , D3DLOCK_DISCARD );
	if( hr != S_OK )
	{
		pitch = 0;
		return NULL;
	}

	pitch = rect.Pitch / 4;

	return (unsigned char *)rect.pBits;
}

void RenderSystemDX9::unLockBuffer()
{
	m_texture->UnlockRect( 0 );
}

void RenderSystemDX9::beginScene()
{
	//if( !m_isValid )
		//m_p3d9Dev->BeginScene();
}

void RenderSystemDX9::endScene()
{
	//if( !m_isValid )
		//m_p3d9Dev->EndScene();
}

void RenderSystemDX9::present()
{
	//背景颜色设置为雾的颜色，防止露出与雾不一样的颜色
	m_p3d9Dev->Clear( 0, NULL, D3DCLEAR_TARGET , 0x00000000, 1.0f, 0 );
	
	HRESULT hr = S_OK;
	hr =m_p3d9Dev->BeginScene();
	m_p3d9Dev->SetTexture( 0 , m_texture );
	//m_p3d9Dev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//m_p3d9Dev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	 
	
	hr =m_p3d9Dev->SetStreamSource( 0 , m_vertexBuff , 0 , sizeof(CUSTOMTEXTUREVERTEX) );
	hr =m_p3d9Dev->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	m_p3d9Dev->SetIndices( m_indexBuff );
	m_p3d9Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , 4 , 0 , 2 );
	//hr =m_p3d9Dev->DrawPrimitive( D3DPT_TRIANGLELIST , 0 , 2 );
	hr =m_p3d9Dev->EndScene();

	if( m_isValid )
		m_p3d9Dev->Present( NULL , NULL , NULL , NULL );
}

bool RenderSystemDX9::initD3d()
{
	//初始化dx9设备
	m_d3d = Direct3DCreate9( D3D_SDK_VERSION );
	if( !m_d3d ) return false;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;  //4倍抗锯齿，设置后还需要在渲染状态中启用
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP ;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	//设置自动深度和模板缓冲区，如果不设置则不使用该功能，必须同时设置缓冲区格式24位深度，8位模板
	//d3dpp.EnableAutoDepthStencil=TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	//这里只能使用D3DCREATE_SOFTWARE_VERTEXPROCESSING，硬件的方式笔记本电脑不能使用
	if( FAILED( m_d3d->CreateDevice(  D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , m_hwnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &d3dpp , &m_p3d9Dev ) ) )
	{
		//创建硬件顶点处理失败了，尝试混合模式
		if( FAILED( m_d3d->CreateDevice(  D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , m_hwnd , D3DCREATE_MIXED_VERTEXPROCESSING , &d3dpp , &m_p3d9Dev ) ) )
		{
			//创建混合模式失败了，尝试软模式
			if( FAILED( m_d3d->CreateDevice(  D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , m_hwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &m_p3d9Dev ) ) )
			{
				return false;
			}
		}
	}

	return true;
}
