#pragma once
//This class is only valid for Linux OS
#if OS_LINUX

#include "WindowSystem/Window.h"

BEGINNAMESPACE

class RenderContext;

class X11Window : public Window {
	friend class Main;
public:
    X11Window();
    ~X11Window();

	inline const WindowDesc& getWindowDesc() const { return m_Desc; }
    inline void* getDisplay() { return m_Display; }
    inline void* getVisualInfo() { return m_VisualInfo; }
    inline void* getFBConfig() { return m_FBConfig; }
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
	virtual bool _impl_createContext(ContextTypeFlags contextType);
private:

private:
    void *m_Display;
    void *m_Window;
    void *m_VisualInfo;
    void *m_FBConfig;

	int32 m_Width, m_Height;
	int32 m_XPos, m_YPos;
	WindowDesc m_Desc;

	typedef Byte ContextStorage[64];
	ContextStorage __contextStorage; //To allow placement new! (Unsecure!)

private:
    //static int32 processMessage(HWND hWnd, uint32 msg, uint64 wParam, int64 lParam);
};

typedef X11Window PlatformWindow;

ENDNAMESPACE

#endif
