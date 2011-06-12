#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "RenderRoot.h"
#include "IoSystem.h"

class Application : public IoSystem::KeyboardEventListener , public IoSystem::MouseEventListener
{
public:
    Application();
    ~Application();

public:
    void init( HWND hwnd );
    void createScene();
    void update();
    void clear();

private:
    //键盘按键回调函数
    void keyPressed( const IoSystem::ISKeyInfo & info ) ;
    void keyReleased( const IoSystem::ISKeyInfo & info ) ;

    //鼠标回调函数
    void MouseMove( const IoSystem::ISMouseInfo& info );
    void MousePressed( const IoSystem::ISMouseInfo& info );
    void MouseReleased( const IoSystem::ISMouseInfo& info );
    enum KeyState
    {
        KS_Forward		= 0x1,
        KS_Back			= 0x2,
        KS_Left			= 0x4,
        KS_Right		= 0x8
    };

    unsigned int m_keyState;
private:
    RenderRoot m_root;

    //输入输出管理器对象
    IoSystem * m_ioSys;
};

#endif //_APPLICATION_H_

