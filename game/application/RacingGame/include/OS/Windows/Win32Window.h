#pragma once
//This class is only valid for Windows OS
#if OS_WINDOWS 

#include "WindowSystem/Window.h"

//Predefs of Win32 Handle
typedef struct HWND__* HWND;
typedef struct HINSTANCE__ * HINSTANCE;
typedef struct HRGN__* HRGN;

BEGINNAMESPACE

class RenderContext;

class Win32Window : public Window {
	friend class Main;
public:
	Win32Window();
	~Win32Window();

	inline const WindowDesc& getWindowDesc() const { return m_Desc; }
protected:
	virtual bool _impl_open(WindowDesc desc);
	virtual bool _impl_isClosed();
	virtual void _impl_setSize(int32 width, int32 height);
	virtual void _impl_getSize(int32& width, int32& height) const;
	virtual void _impl_setPosition(int32 xPos, int32 yPos);
	virtual void _impl_getPosition(int32& xPos, int32& yPos) const;
	virtual void _impl_getFramebufferSize(int32& width, int32& height) const;
	virtual void _impl_setTitle(const ansichar* title);
	virtual void _impl_minimize();
	virtual void _impl_maximize();
	virtual void _impl_restore();
	virtual bool _impl_isMinimized() const;
	virtual bool _impl_isMaximized() const;
	virtual void _impl_show();
	virtual void _impl_hide();
	virtual bool _impl_isVisible() const;
	virtual bool _impl_hasFocus() const;
	virtual bool _impl_isResizeable() const;
	virtual bool _impl_isDecorated() const;
	virtual void _impl_getOpenGLContext(int32& major, int32& minor, int32& revision) const;
	virtual bool _impl_isForwardCompatible() const;
	virtual bool _impl_isDebugContext() const;
	virtual FramebufferDesc _impl_getWindowFramebufferDesc() const;
	virtual void _impl_swapBuffers();
	virtual void _impl_destroy();
	virtual void* _impl_getNativeHandle();
	virtual bool _impl_createContext(RenderEngineTypeFlags contextType);
private:
	size_type Win32Window::getBorderSize() const;
	HRGN makeWindowRegionObject(int32 width, int32 height) const;
	void adjustWindowRegion(int32 width, int32 height);

private:
	HWND m_Handle;
	HINSTANCE m_HInstance;
	int32 m_Width, m_Height;
	int32 m_XPos, m_YPos;
	WindowDesc m_Desc;

	typedef Byte ContextStorage[64];
	ContextStorage __contextStorage; //To allow placement new! (Unsecure!)

private:
	static int32 processMessage(HWND hWnd, uint32 msg, uint64 wParam, int64 lParam);
};

typedef Win32Window PlatformWindow;

ENDNAMESPACE

#endif