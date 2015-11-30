#pragma once

#include <Delegates/Delegate.h>
#include <Size.h>
#include <Point.h>

#include <Utilities/Flags.h>

#include <WindowSystem/WindowDesc.h>

BEGINNAMESPACE

typedef struct {
	int32 red_bits, green_bits, blue_bits, alpha_bits, depth_bits, stencil_bits, msaa_samples;
} FramebufferDesc;

DECLARE_EXCLUSIVE_FLAGS(WindowMode, Unknown, Fullscreen, Windowed, WindowedFullscreen);
DECLARE_EXCLUSIVE_FLAGS(ContextType, OpenGL, None);

class Window;
class RenderContext;

typedef struct {
	Delegate<void(Window*)> onClose;
	Delegate<void(Window*, const Size2i&)> onResize;
	Delegate<void(Window*, const Size2i&)> onFramebufferResize;
	Delegate<void(Window*, const Point2i&)> onPositionChanged;
	Delegate<void(Window*, bool)> onMinimizeResized;
	Delegate<void(Window*, bool)> onFocusChanged;
} WindowDelegates;

class Window {
public:
	typedef WindowDelegates Delegates;
public:
	virtual ~Window();

	bool open() { return _impl_open(WindowDesc::getDefault()); }
	bool open(WindowDesc desc) { return _impl_open(desc); }
	bool isClosed() { return _impl_isClosed(); }

	//
	/// window size
	inline void setSize(int32 width, int32 height) { _impl_setSize(width, height);	}
	inline void setSize(const Size2i& size) { _impl_setSize(size.width, size.height); }
	inline Size2i getSize() const { Size2i s; _impl_getSize(s.width, s.height); return s; };
	inline void getSize(int32& width, int32& height) const { _impl_getSize(width, height); }

	//
	/// window framebuffer size
	Size2i getFramebufferSize() const { Size2i f; _impl_getFramebufferSize(f.width, f.height); }
	void getFramebufferSize(int32& width, int32& height) const { _impl_getFramebufferSize(width, height); }

	//
	/// window position
	inline void setPosition(int32 xPos, int32 yPos) { _impl_setPosition(xPos, yPos); }
	inline void setPosition(const Point2i& position) { _impl_setPosition(position.x, position.y); }
	inline Point2i getPosition() const { Point2i p; _impl_getPosition(p.x, p.y); return p; }
	inline void getPosition(int32& x, int32& y) const { _impl_getPosition(x, y); }

	//
	/// window title
	inline void setTitle(const ansichar* title) { _impl_setTitle(title); }

	//minimize / restore
	inline void minimize() { _impl_minimize(); }
	inline void restore() { _impl_restore(); }
	inline bool isMinimized() const { return _impl_isMinimized(); }

	//show / hide
	inline void show() { _impl_show(); }
	inline void hide() { _impl_hide(); }
	inline bool isVisible() const { return _impl_isVisible(); }

	//focus
	inline bool hasFocus() const { return _impl_hasFocus(); }

	//resizable
	inline bool isResizeable() const { return _impl_isResizeable(); }

	//decorated (border, close-button...)
	inline bool isDecorated() const { return _impl_isDecorated(); }

	//opengl version for the window context
	inline void getOpenGLContext(int32& major, int32& minor, int32& revision) const { _impl_getOpenGLContext(major, minor, revision); }

	//is forward compat
	inline bool isForwardCompatible() const { return _impl_isForwardCompatible(); }

	//is debug context
	inline bool isDebugContext() const { return _impl_isDebugContext(); }

	//framebuffer desc
	inline FramebufferDesc getWindowFramebufferDesc() const { return _impl_getWindowFramebufferDesc(); }

	//swap buffer
	inline void swapBuffers() { _impl_swapBuffers(); }
	
	//destroy
	inline void destroy() { return _impl_destroy(); }
	
	//native handle
	inline void* getNativeHandle() { return _impl_getNativeHandle(); }

	//render context creation
	inline RenderContext* createContext(ContextTypeFlags contextType = ContextType::OpenGL) { if (_impl_createContext(contextType)) return m_RenderContext; return nullptr; }

	//
	/// window delegates
	inline Delegates* getDelegates() { return m_delegates; }
	inline const Delegates* getDelegates() const { return m_delegates; }
	inline void setDelegates(Delegates* delegates) { m_delegates = delegates; }

	inline RenderContext* getRenderContext() { return m_RenderContext; }
	
protected:
	virtual bool _impl_open(WindowDesc desc) = 0;
	virtual bool _impl_isClosed() = 0;
	virtual void _impl_setSize(int32 width, int32 height) = 0;
	virtual void _impl_getSize(int32& width, int32& height) const = 0;
	virtual void _impl_setPosition(int32 xPos, int32 yPos) = 0;
	virtual void _impl_getPosition(int32& width, int32& height) const = 0;
	virtual void _impl_getFramebufferSize(int32& width, int32& height) const = 0;
	virtual void _impl_setTitle(const ansichar* title) = 0;
	virtual void _impl_minimize() = 0;
	virtual void _impl_restore() = 0;
	virtual bool _impl_isMinimized() const = 0;
	virtual void _impl_show() = 0;
	virtual void _impl_hide() = 0;
	virtual bool _impl_isVisible() const = 0;
	virtual bool _impl_hasFocus() const = 0;
	virtual bool _impl_isResizeable() const = 0;
	virtual bool _impl_isDecorated() const = 0;
	virtual void _impl_getOpenGLContext(int32& major, int32& minor, int32& revision) const = 0;
	virtual bool _impl_isForwardCompatible() const = 0;
	virtual bool _impl_isDebugContext() const = 0;
	virtual FramebufferDesc _impl_getWindowFramebufferDesc() const = 0;
	virtual void _impl_swapBuffers() = 0;
	virtual void _impl_destroy() = 0;
	virtual void* _impl_getNativeHandle() = 0;
	virtual bool _impl_createContext(ContextTypeFlags contextType) = 0;
	
protected:
	WindowModeFlags m_WindowMode;
	RenderContext* m_RenderContext;
	typedef void* _wndHandle;
	_wndHandle m_handle;
	Delegates* m_delegates;
private:
	/*
	static void _CallCloseCallback(void* ptr);
	static void _CallSizeCallback(void* ptr, int32 width, int32 height);
	static void _CallFramebufferSizeCallback(void* ptr, int32 width, int32 height);
	static void _CallPositionChanged(void* ptr, int32 posX, int32 posY);
	static void _CallMinimizedRestored(void* ptr, int32 minimized);
	static void _CallFocusChanged(void* ptr, int32 focus);
	//*/
};

ENDNAMESPACE

#if OS_WINDOWS
#	include "OS/Windows/Window.h"
#elif OS_LINUX
#   include "OS/Linux/X11Window.h"
#endif
