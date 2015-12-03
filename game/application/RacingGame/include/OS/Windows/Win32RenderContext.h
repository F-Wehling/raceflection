#pragma once
//This class is only valid for Windows OS
#if OS_WINDOWS 

#include "RenderSystem/RenderContext.h"

//Predefs of Win32 Handle
typedef struct HDC__* HDC;
typedef struct HGLRC__* HGLRC;

BEGINNAMESPACE

class Win32Window;

class Win32RenderGLContext : public RenderContext {
public:
	Win32RenderGLContext(Win32Window* contextForWindow);
	~Win32RenderGLContext();
protected:
	virtual void _swapBuffer();
	virtual void _makeCurrent();
	virtual bool _valid();
private:
	HDC m_DeviceContext;
	HGLRC m_GLContext;
	int32 m_PixelFormat;
	Win32Window * m_RefWindow;
};

typedef Win32RenderGLContext PlatformGLContext;

ENDNAMESPACE

#endif