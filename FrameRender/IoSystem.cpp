#include "stdafx.h"
#include "IoSystem.h"

extern HINSTANCE hInst; //实例句柄

IoSystem::IoSystem( HWND hwnd )
:m_hwnd(hwnd),m_isInit(false) ,m_keyboardListener(NULL) ,m_mouseListener(NULL)
{
	//初始化dxinput 设备
	DirectInput8Create( hInst , DIRECTINPUT_VERSION , IID_IDirectInput8 , (void**)&m_inputDev ,NULL );

	//初始化鼠标初值
	m_mouseX = 0;
	m_mouseY = 0;
	m_leftBtnDown = false;
	m_rightBtnDown = false;
}

IoSystem::~IoSystem()
{
	if( m_inputDev )
	{
		m_inputDev->Release();
		m_inputDev = NULL; 
	}	
}

bool IoSystem::initial()
{
	if( m_isInit ) return true;

	if( !_initKeyboardDev() )
		return false;

	if( !_initMouseDev() )
		return false;

	return m_isInit = true;
}

bool IoSystem::unInitial()
{
	_clearKeyboardDev();
	_clearMouseDev();

	return true;
}

bool IoSystem::_initKeyboardDev()
{
	//初始化键盘设备
	HRESULT hr = m_inputDev->CreateDevice(GUID_SysKeyboard, &m_lpDIDevice, NULL); 
	if FAILED(hr) 
	{ 
		return false; 
	} 
	
	//设置键盘信息格式
	hr = m_lpDIDevice->SetDataFormat(&c_dfDIKeyboard); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//设置消息源
	hr = m_lpDIDevice->SetCooperativeLevel(m_hwnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//获取设备
	if ( m_lpDIDevice )
	{
		hr = m_lpDIDevice->Acquire(); 
	}

	return true;
}

bool IoSystem::_reAcquireKeyboard()
{
	m_lpDIDevice->Acquire(); 
	return true;
}

bool IoSystem::_updateKeyboard()
{
	//重置所有键盘按下状态
	for( downMap::iterator it = m_downMap.begin(); it != m_downMap.end() ; it++ )
	{
		it->second = false; 
	}
	
	//处理这轮键盘事件
#define KEYDOWN(name, key) (name[key] & 0x80) 

	char     buffer[256]; 
	HRESULT  hr; 
	hr = m_lpDIDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
	
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) 
	{
		_reAcquireKeyboard();
		return true;
	}

	if FAILED(hr) 
	{ 
		return false; 
	} 
	
	// Turn the spaceship right or left 
	if (KEYDOWN(buffer, DIK_A))
	{
		keyProcess( IS_A );
	}

	if(KEYDOWN(buffer, DIK_S))
	{
		keyProcess( IS_S );
	}

	if (KEYDOWN(buffer, DIK_D))
	{
		keyProcess( IS_D );
	}

	if (KEYDOWN(buffer, DIK_W))
	{
		keyProcess( IS_W );
	}
	
	//发送按键弹起的事件
	for( downMap::iterator it = m_downMap.begin(); it != m_downMap.end() ; )
	{
		if( !it->second )
		{
			//表示这个键上一轮按下，这一轮弹起了
			if( m_keyboardListener )
			{
				//发送事件
				ISKeyInfo info;
				info.keycode = it->first;
				m_keyboardListener->keyReleased( info );
			}
			//删除对应对象
			it = m_downMap.erase( it );
		}
		else
			it++ ;
	}

	return true;
}

bool IoSystem::_clearKeyboardDev()
{
	if (m_lpDIDevice) 
	{ 
		// Always unacquire device before calling Release(). 
		m_lpDIDevice->Unacquire(); 
		m_lpDIDevice->Release();
		m_lpDIDevice = NULL; 
	} 

	return true;
}

bool IoSystem::_initMouseDev()
{
	//初始化鼠标设备
	HRESULT hr = m_inputDev->CreateDevice(GUID_SysMouse, &m_lpMouseDIDevice, NULL); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//设置键盘信息格式
	hr = m_lpMouseDIDevice->SetDataFormat(&c_dfDIMouse); 
	if FAILED(hr) 
	{ 
		return false; 
	} 
	
	//设置消息源
	hr = m_lpMouseDIDevice->SetCooperativeLevel(m_hwnd, 
		DISCL_FOREGROUND | DISCL_EXCLUSIVE); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//设置鼠标信息缓冲区信息
	DIPROPDWORD dipdw;
	// the header
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	// the data
	dipdw.dwData            = 16;

	//设置鼠标设备属性
	hr = m_lpMouseDIDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)) 
	{
		return false;
	}
	
	//设置鼠标位置为绝对坐标
	dipdw.dwData            = DIPROPAXISMODE_ABS;
	hr = m_lpMouseDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	if (FAILED(hr)) 
	{
		return false;
	}

	//获取设备
	if ( m_lpMouseDIDevice ) 
		m_lpMouseDIDevice->Acquire(); 

	return true;
}

bool IoSystem::_reAcquireMouse()
{
	
	m_lpMouseDIDevice->Acquire();
	return true;
}	


bool IoSystem::_updateMouse()
{
	DIDEVICEOBJECTDATA diBuff[16];
	DWORD entries = 16;
	
	//获取鼠标设备状态
	HRESULT hr = m_lpMouseDIDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA) , 	diBuff, &entries , 0 );
	
	//表示设备丢失，重新获取
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) 
	{
		_reAcquireMouse();
		return true;
	}
	
	//表示无数据
	if (FAILED(hr) || entries == 0) 
	{
		return true;
	}
	ISMouseInfo info;
	memset( &info , 0 , sizeof(ISMouseInfo) );

	for( DWORD i = 0 ; i < entries ; i++ )
	{
		//事件分发
		switch (diBuff[i].dwOfs) 
		{
			// Mouse horizontal motion
		case DIMOFS_X: 
			info.relPosX += ( m_mouseX - diBuff[i].dwData ) ;
			m_mouseX = diBuff[i].dwData; 
			break;

			// Mouse vertical motion
		case DIMOFS_Y:
			info.relPoxY += ( m_mouseY - diBuff[i].dwData );
			m_mouseY = diBuff[i].dwData; 
			break; 

		case DIMOFS_BUTTON0: //左键事件
			if (diBuff[i].dwData & 0x80)  
			{
				//表示左键按下,只有之前没按下才发消息
				if( !m_leftBtnDown )
				{				
					info.posX = m_mouseX;
					info.posY = m_mouseY;
					info.mouseCode = IS_LEFTBTN;
					info.mouseData = 1;
					if( m_mouseListener )
						m_mouseListener->MousePressed( info );
					m_leftBtnDown = true;
				}
			}
			else
			{
				//表示左键抬起,只有之前没抬起才发消息
				if( m_leftBtnDown )
				{				
					info.posX = m_mouseX;
					info.posY = m_mouseY;
					info.mouseCode = IS_LEFTBTN;
					info.mouseData = 0;
					if( m_mouseListener )
						m_mouseListener->MousePressed( info );
					m_leftBtnDown = false;
				}
			}
			break;        
		case DIMOFS_BUTTON1:  //右键事件
			if( (diBuff[i].dwData & 0x80) )
			{  
				//表示右键按下,只有之前没按下才发消息
				if( !m_rightBtnDown )
				{				
					info.posX = m_mouseX;
					info.posY = m_mouseY;
					info.mouseCode = IS_RIGHTBTN;
					info.mouseData = 1;
					if( m_mouseListener )
						m_mouseListener->MousePressed( info );
					m_rightBtnDown = true;
				}
			}
			else
			{
				//表示右键抬起,只有之前没抬起才发消息
				if( m_rightBtnDown )
				{				
					info.posX = m_mouseX;
					info.posY = m_mouseY;
					info.mouseCode = IS_RIGHTBTN;
					info.mouseData = 0;
					if( m_mouseListener )
						m_mouseListener->MousePressed( info );
					m_rightBtnDown = false;
				}
			}
			break;

		}
	}
	info.mouseCode = IS_NOBTN;
	info.posX = m_mouseX;
	info.posY = m_mouseY;

	if( m_mouseListener )
		m_mouseListener->MouseMove( info );
	
	return true;
}

bool IoSystem::_clearMouseDev()
{
	if ( m_lpMouseDIDevice ) 
	{ 
		// Always unacquire device before calling Release(). 
		m_lpMouseDIDevice->Unacquire(); 
		m_lpMouseDIDevice->Release();
		m_lpMouseDIDevice = NULL; 
	} 
	return true;
}

//更新
void IoSystem::update()
{
	//更新这轮键盘设备
	_updateKeyboard();

	_updateMouse();
}

void IoSystem::keyProcess( IS_KEY_TYPE type )
{
	downMap::iterator it = m_downMap.find( type );
	if( it != m_downMap.end() )
	{
		//表示这个键已经按下了，则更新状态，表示仍然按下
		it->second = true;
	}
	else
	{
		//表示这个键没有按下过，则记录并发事件
		m_downMap.insert( std::make_pair( type , true ) );
		
		if( m_keyboardListener )
		{
			//发送事件
			ISKeyInfo info;
			info.keycode = type;
			m_keyboardListener->keyPressed( info );
		}
	}
}

