#include "stdafx.h"
#include "Application.h"
#include "RenderSystem.h"
#include "RenderObject.h"
#include "Camera.h"

Application::Application()
{

}

Application::~Application()
{

}

void Application::init( HWND hwnd )
{
    m_root.init( hwnd , 256 , 256 );
   
    m_ioSys = new IoSystem( hwnd );
    m_ioSys->initial();
    m_ioSys->setKeyboardListener( this );
    m_ioSys->setMouseListener( this );
}

void Application::createScene()
{
    //添加1个渲染对象
    
    RenderObject * pObj = new RenderObject();
    pObj->manualCreateObject( 8 , 36 );

    VERTEX_TYPE vec;
    vec.x = 0;
    vec.y = 0;
    vec.z = 0;
    vec.w = 1;
    vec.r = 1.0f;
    vec.g = 1.0f;
    vec.b = 1.0f;
    pObj->addVertex( vec );
    
    vec.x = 10;
    vec.y = 0;
    vec.z = 0;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 10;
    vec.y = 10;
    vec.z = 0;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 0;
    vec.y = 10;
    vec.z = 0;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 0;
    vec.y = 0;
    vec.z = 10;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 10;
    vec.y = 0;
    vec.z = 10;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 10;
    vec.y = 10;
    vec.z = 10;
    vec.w = 1;
    pObj->addVertex( vec );

    vec.x = 0;
    vec.y = 10;
    vec.z = 10;
    vec.w = 1;
    pObj->addVertex( vec );

    pObj->addFaceIndex( 0 , 3 , 2 );
    pObj->addFaceIndex( 0 , 2 , 1 );

    pObj->addFaceIndex( 1 , 2 , 6 );
    pObj->addFaceIndex( 1 , 6 , 5 );

    pObj->addFaceIndex( 3 , 4 , 7 );
    pObj->addFaceIndex( 0 , 4 , 3 );

    pObj->addFaceIndex( 4 , 5 , 6 );
    pObj->addFaceIndex( 4 , 6 , 7 );

    pObj->addFaceIndex( 2 , 3 , 7 );
    pObj->addFaceIndex( 2 , 7 , 6 );

    pObj->addFaceIndex( 1 , 4 , 0 );
    pObj->addFaceIndex( 1 , 5 , 4 );

    pObj->setPosition( Vector3( -5.0f , -5.0f , 0.0f ) );

    m_root.addRenderObject( pObj );
    
    /*
    RenderObject * pObj = new RenderObject();
    pObj->manualCreateObject( 3 , 3 );
    VERTEX_TYPE vec;
    vec.x = 0;
    vec.y = 0;
    vec.z = 0;
    vec.w = 1;
    vec.r = 1.0f;
    vec.g = 0.0f;
    vec.b = 0.0f;
    pObj->addVertex( vec );

    vec.x = 0;
    vec.y = 10;
    vec.z = 0;
    vec.w = 1;
    vec.r = 0.0f;
    vec.g = 1.0f;
    vec.b = 0.0f;
    pObj->addVertex( vec );

    vec.x = 5;
    vec.y = 0;
    vec.z = 0;
    vec.w = 1;
    vec.r = 0.0f;
    vec.g = 0.0f;
    vec.b = 1.0f;
    pObj->addVertex( vec );

    pObj->addFaceIndex( 0 , 1 , 2 );
    pObj->setPosition( Vector3( 0.0f , 0.0f , 10.0f ) );
    m_root.addRenderObject( pObj );
*/
    //设置摄像机的属性
    Camera * pCam = new Camera( 5.0f ,1000.0f , 1.57f ) ;
    pCam->lookAt( Vector3( 0 , 0 , 0 ) );
    //pCam->setPosition( Vector3( 30 , 30 , -30 ) );
    pCam->setPosition( Vector3( 0 , 0 , -15 ) );
    m_root.setCamera( pCam );
}

void Application::update()
{
    m_root.renderOneFrame();

    if( m_ioSys )
        m_ioSys->update();
    
    Camera * pCam = m_root.getCamera();

    if( m_keyState & KS_Forward )
    {
        //pCam->pitch( -0.01f );
        pCam->setPosition( pCam->getPosition() + pCam->getOrientation() * 0.3f );
     }
    else if( m_keyState & KS_Back )
    {
        //pCam->pitch( 0.01f );
        pCam->setPosition( pCam->getPosition() - pCam->getOrientation() * 0.3f );
    }
    else if( m_keyState & KS_Left )
    {
        pCam->yaw( -0.01f );
        //pCam->setPosition( pCam->getPosition() + Vector3( 0.0f,0.0f,-0.1f) );
    }
    else if( m_keyState & KS_Right )
    {
        pCam->yaw( 0.01f );
        //pCam->setPosition( pCam->getPosition() + Vector3( 0.0f,0.0f,-0.1f) );
    }
}

void Application::clear()
{

}


void Application::keyPressed( const IoSystem::ISKeyInfo & info )
{
    switch ( info.keycode )
    {
    case IoSystem::IS_W:
        m_keyState |= KS_Forward;
        break;
    case IoSystem::IS_S:
        m_keyState |= KS_Back;
        break;
    case IoSystem::IS_D:
        m_keyState |= KS_Right;
        break;
    case IoSystem::IS_A:
        m_keyState |= KS_Left;
        break;
    }
}

void Application::keyReleased( const IoSystem::ISKeyInfo & info ) 
{
    switch ( info.keycode )
    {
    case IoSystem::IS_W:
        m_keyState &= ~KS_Forward;
        break;
    case IoSystem::IS_S:
        m_keyState &= ~KS_Back;
        break;
    case IoSystem::IS_D:
        m_keyState &= ~KS_Right;
        break;
    case IoSystem::IS_A:
        m_keyState &= ~KS_Left;
        break;
    }
}

void Application::MouseMove( const IoSystem::ISMouseInfo& info )
{
    Camera * pCam = m_root.getCamera();
    if( info.relPosX > 0 )
    {
        pCam->yaw( -0.01f );
    }
    else if( info.relPosX < 0 )
    {
        pCam->yaw( 0.01f );
    }

    if( info.relPoxY >0 )
    {
        pCam->pitch( 0.01f );
    }
    else if( info.relPoxY < 0 )
    {
        pCam->pitch( -0.01f );
    }
}

void Application::MousePressed( const IoSystem::ISMouseInfo& info )
{

}

void Application::MouseReleased( const IoSystem::ISMouseInfo& info )
{

}