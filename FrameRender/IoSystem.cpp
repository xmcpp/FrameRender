#include "stdafx.h"
#include "IoSystem.h"

extern HINSTANCE hInst; //ʵ�����

IoSystem::IoSystem( HWND hwnd )
:m_hwnd(hwnd),m_isInit(false) ,m_keyboardListener(NULL) ,m_mouseListener(NULL)
{
	//��ʼ��dxinput �豸
	DirectInput8Create( hInst , DIRECTINPUT_VERSION , IID_IDirectInput8 , (void**)&m_inputDev ,NULL );

	//��ʼ������ֵ
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
	//��ʼ�������豸
	HRESULT hr = m_inputDev->CreateDevice(GUID_SysKeyboard, &m_lpDIDevice, NULL); 
	if FAILED(hr) 
	{ 
		return false; 
	} 
	
	//���ü�����Ϣ��ʽ
	hr = m_lpDIDevice->SetDataFormat(&c_dfDIKeyboard); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//������ϢԴ
	hr = m_lpDIDevice->SetCooperativeLevel(m_hwnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//��ȡ�豸
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
	//�������м��̰���״̬
	for( downMap::iterator it = m_downMap.begin(); it != m_downMap.end() ; it++ )
	{
		it->second = false; 
	}
	
	//�������ּ����¼�
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
	
	//���Ͱ���������¼�
	for( downMap::iterator it = m_downMap.begin(); it != m_downMap.end() ; )
	{
		if( !it->second )
		{
			//��ʾ�������һ�ְ��£���һ�ֵ�����
			if( m_keyboardListener )
			{
				//�����¼�
				ISKeyInfo info;
				info.keycode = it->first;
				m_keyboardListener->keyReleased( info );
			}
			//ɾ����Ӧ����
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
	//��ʼ������豸
	HRESULT hr = m_inputDev->CreateDevice(GUID_SysMouse, &m_lpMouseDIDevice, NULL); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//���ü�����Ϣ��ʽ
	hr = m_lpMouseDIDevice->SetDataFormat(&c_dfDIMouse); 
	if FAILED(hr) 
	{ 
		return false; 
	} 
	
	//������ϢԴ
	hr = m_lpMouseDIDevice->SetCooperativeLevel(m_hwnd, 
		DISCL_FOREGROUND | DISCL_EXCLUSIVE); 
	if FAILED(hr) 
	{ 
		return false; 
	} 

	//���������Ϣ��������Ϣ
	DIPROPDWORD dipdw;
	// the header
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	// the data
	dipdw.dwData            = 16;

	//��������豸����
	hr = m_lpMouseDIDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)) 
	{
		return false;
	}
	
	//�������λ��Ϊ��������
	dipdw.dwData            = DIPROPAXISMODE_ABS;
	hr = m_lpMouseDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	if (FAILED(hr)) 
	{
		return false;
	}

	//��ȡ�豸
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
	
	//��ȡ����豸״̬
	HRESULT hr = m_lpMouseDIDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA) , 	diBuff, &entries , 0 );
	
	//��ʾ�豸��ʧ�����»�ȡ
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) 
	{
		_reAcquireMouse();
		return true;
	}
	
	//��ʾ������
	if (FAILED(hr) || entries == 0) 
	{
		return true;
	}
	ISMouseInfo info;
	memset( &info , 0 , sizeof(ISMouseInfo) );

	for( DWORD i = 0 ; i < entries ; i++ )
	{
		//�¼��ַ�
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

		case DIMOFS_BUTTON0: //����¼�
			if (diBuff[i].dwData & 0x80)  
			{
				//��ʾ�������,ֻ��֮ǰû���²ŷ���Ϣ
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
				//��ʾ���̧��,ֻ��֮ǰû̧��ŷ���Ϣ
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
		case DIMOFS_BUTTON1:  //�Ҽ��¼�
			if( (diBuff[i].dwData & 0x80) )
			{  
				//��ʾ�Ҽ�����,ֻ��֮ǰû���²ŷ���Ϣ
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
				//��ʾ�Ҽ�̧��,ֻ��֮ǰû̧��ŷ���Ϣ
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

//����
void IoSystem::update()
{
	//�������ּ����豸
	_updateKeyboard();

	_updateMouse();
}

void IoSystem::keyProcess( IS_KEY_TYPE type )
{
	downMap::iterator it = m_downMap.find( type );
	if( it != m_downMap.end() )
	{
		//��ʾ������Ѿ������ˣ������״̬����ʾ��Ȼ����
		it->second = true;
	}
	else
	{
		//��ʾ�����û�а��¹������¼�����¼�
		m_downMap.insert( std::make_pair( type , true ) );
		
		if( m_keyboardListener )
		{
			//�����¼�
			ISKeyInfo info;
			info.keycode = type;
			m_keyboardListener->keyPressed( info );
		}
	}
}

