#pragma once

class IoSystem
{
public:
	IoSystem( HWND hwnd );
	~IoSystem();


	enum IS_KEY_TYPE
	{
		IS_A = 1,
		IS_S ,
		IS_D ,
		IS_W
	};
	
	enum IS_MOUSE_TYPE
	{
		IS_NOBTN = 1,
		IS_LEFTBTN = 2,
		IS_RIGHTBTN ,
		IS_MIDBTN
	};

	//��¼��ǰ��������Ϣ
	struct ISKeyInfo
	{
		IS_KEY_TYPE keycode; 
	};
	
	//��¼��ǰ������Ϣ
	struct ISMouseInfo
	{
		long posX; //��¼��굱ǰxλ��
		long posY; //��¼��굱ǰyλ��
		long relPosX; //��¼��굱ǰxƫ��
		long relPoxY; //��¼��굱ǰyƫ��
		IS_MOUSE_TYPE mouseCode;
		short mouseData; //0��ʾ̧��1��ʾ����
	};

	class KeyboardEventListener
	{
	public:
		KeyboardEventListener(){}
		virtual ~KeyboardEventListener(){}
	public:
		virtual void keyPressed( const ISKeyInfo & info ) = 0;
		virtual void keyReleased( const ISKeyInfo & info ) = 0;
	};

	class MouseEventListener
	{
	public:
		MouseEventListener(){}
		virtual ~MouseEventListener(){}
	public:
		virtual void MouseMove( const ISMouseInfo& info ) = 0;
		virtual void MousePressed( const ISMouseInfo& info ) = 0;
		virtual void MouseReleased( const ISMouseInfo& info ) = 0;
	};
private:

public:
	bool initial();
	bool unInitial();
	
	//���ü��̼����߶���
	void setKeyboardListener( KeyboardEventListener * listener ){ m_keyboardListener = listener ;}
	
	//�����������߶���
	void setMouseListener( MouseEventListener * listener ){ m_mouseListener = listener ;}

	//����
	void update();
	
private:
	bool _initKeyboardDev();
	bool _initMouseDev();

	bool _updateKeyboard();
	bool _updateMouse();
	
	bool _clearKeyboardDev();
	bool _clearMouseDev();
	
	bool _reAcquireMouse();
	bool _reAcquireKeyboard();

	//��������
	void keyProcess( IS_KEY_TYPE type );

private:
	typedef std::map<IS_KEY_TYPE,bool> downMap;
	std::map<IS_KEY_TYPE,bool> m_downMap; //��¼��ǰ�Ѿ����µļ�,�����bool�������ڼ�¼��һ��ѭ�������Ƿ��а��£����û�У�������release�¼�
	
private:
	//Dirext�豸ָ��
	LPDIRECTINPUT8 m_inputDev ;

	//���������豸ָ��
	LPDIRECTINPUTDEVICE8  m_lpDIDevice;

	//��������豸ָ��
	LPDIRECTINPUTDEVICE8  m_lpMouseDIDevice;

	HWND m_hwnd; //������

	bool m_isInit; //��¼�Ƿ������ɹ�

	KeyboardEventListener * m_keyboardListener; //��¼������Ϣ������
	MouseEventListener * m_mouseListener; //��¼�����Ϣ������

	DWORD m_mouseX; //��¼���x����
	DWORD m_mouseY; //��¼���y����
	bool m_leftBtnDown; //��¼����Ƿ񱻰���
	bool m_rightBtnDown; //��¼�Ҽ��Ƿ񱻰���
};