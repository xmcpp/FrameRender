#include "stdafx.h"
#include "RenderRoot.h"
#include "RenderSystem.h"
#include "RenderSystemGDI.h"
#include "RenderQueue.h"
#include "RenderObject.h"
#include "RenderInput.h"
#include "Camera.h"

RenderRoot::RenderRoot()
:m_renderQueue(NULL),m_cam(NULL)
{

}

RenderRoot::~RenderRoot()
{
    if( m_renderQueue )
    {
        delete m_renderQueue;
        m_renderQueue = NULL;
    }
}

void RenderRoot::init( HWND hwnd , int width , int height )
{
	//创建基于GDI的渲染系统
	RenderSystem * sys = new RenderSystemGDI( hwnd );
	sys->createRenderSystem( width , height );

	m_renderQueue = new RenderQueue();
	m_renderQueue->setRenderSystem( sys );
    m_renderQueue->enableDrawFrame( false );
    m_renderQueue->setCullingType(CCW);
    m_renderQueue->setFillType( FILL_WIREFRAME );
}

void RenderRoot::renderOneFrame()
{
    m_renderQueue->beginScene();
	m_renderQueue->setViewMat( m_cam->getViewMat() );
	m_renderQueue->setProjectMat( m_cam->getProjectMat() );

	for( int i = 0 ; i < m_renderObjVec.size() ; i++ )
	{
        m_renderQueue->setWorldMat( m_renderObjVec[i]->getWorldMatrix() );
        m_renderQueue->drawRenderInput( m_renderObjVec[i]->getBindRenderInput() );
	}
    m_renderQueue->endScene();
    m_renderQueue->present();

    
}

void RenderRoot::addRenderObject( RenderObject * pObj )
{
    m_renderObjVec.push_back( pObj );
}



