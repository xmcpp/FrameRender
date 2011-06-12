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

	//记录当前按键的信息
	struct ISKeyInfo
	{
		IS_KEY_TYPE keycode; 
	};
	
	//记录当前鼠标的信息
	struct ISMouseInfo
	{
		long posX; //记录鼠标当前x位置
		long posY; //记录鼠标当前y位置
		long relPosX; //记录鼠标当前x偏移
		long relPoxY; //记录鼠标当前y偏移
		IS_MOUSE_TYPE mouseCode;
		short mouseData; //0表示抬起，1表示按下
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
	
	//设置键盘监听者对象
	void setKeyboardListener( KeyboardEventListener * listener ){ m_keyboardListener = listener ;}
	
	//设置鼠标监听者对象
	void setMouseListener( MouseEventListener * listener ){ m_mouseListener = listener ;}

	//更新
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

	//负责处理按键
	void keyProcess( IS_KEY_TYPE type );

private:
	typedef std::map<IS_KEY_TYPE,bool> downMap;
	std::map<IS_KEY_TYPE,bool> m_downMap; //记录当前已经按下的键,后面的bool类型用于记录在一次循环里面是否有按下，如果没有，则会调用release事件
	
private:
	//Dirext设备指针
	LPDIRECTINPUT8 m_inputDev ;

	//键盘输入设备指针
	LPDIRECTINPUTDEVICE8  m_lpDIDevice;

	//鼠标输入设备指针
	LPDIRECTINPUTDEVICE8  m_lpMouseDIDevice;

	HWND m_hwnd; //窗体句柄

	bool m_isInit; //记录是否启动成功

	KeyboardEventListener * m_keyboardListener; //记录键盘信息监听者
	MouseEventListener * m_mouseListener; //记录鼠标信息监听者

	DWORD m_mouseX; //记录鼠标x坐标
	DWORD m_mouseY; //记录鼠标y坐标
	bool m_leftBtnDown; //记录左键是否被按下
	bool m_rightBtnDown; //记录右键是否被按下
};