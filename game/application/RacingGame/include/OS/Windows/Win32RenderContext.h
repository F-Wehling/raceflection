#pragma once
//This class is only valid for Windows OS
#if OS_WINDOWS 

#include "RenderSystem/RenderContext.h"

//Predefs of Win32 Handle
typedef struct HDC__* HDC;
typedef struct HGLRC__* HGLRC;

BEGINNAMESPACE

class Win32Window;

class Win32RenderContextGL : public RenderContext {
public:
	Win32RenderContextGL(Win32Window* contextForWindow);
	~Win32RenderContextGL();
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

class Win32RenderContextNull : public RenderContext {
public:
	Win32RenderContextNull(Win32Window* contextForWindow);
	~Win32RenderContextNull();
protected:
	virtual void _swapBuffer();
	virtual void _makeCurrent();
	virtual bool _valid();
private:

};

ENDNAMESPACE

#endif