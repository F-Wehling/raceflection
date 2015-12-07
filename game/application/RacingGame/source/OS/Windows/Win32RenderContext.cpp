#if OS_WINDOWS

#include "OS/Windows/Win32RenderContext.h"
#include "OS/Windows/MinWindows.h"
#include "OS/Windows/Win32Window.h"
#include "WindowSystem/WindowDesc.h"

#include "Logging.h"

BEGINNAMESPACE

Win32RenderContextGL::Win32RenderContextGL(Win32Window* contextForWindow)
	: m_RefWindow(contextForWindow),
	 m_DeviceContext(0),
	 m_GLContext(0)
{
	HWND hWnd = (HWND)contextForWindow->getNativeHandle();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | CS_OWNDC); // attach the CS_OWNDC

	WindowDesc desc = contextForWindow->getWindowDesc();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		desc.red_bits, 0, 
		desc.green_bits, 0, 
		desc.blue_bits, 0,
		desc.alpha_bits, 0,
		desc.red_accum_bits + desc.green_accum_bits + desc.blue_accum_bits + desc.alpha_accum_bits,
		desc.red_accum_bits,
		desc.green_accum_bits,
		desc.blue_accum_bits,
		desc.alpha_accum_bits,
		desc.depth_bits,
		desc.stencil_bits,
		0, //0 as not supported for WGL-Context
		PFD_MAIN_PLANE, //ignored 
		0, 0, 0, 0 //unused or even ignored
	};

	m_DeviceContext = GetDC(hWnd);

	m_PixelFormat = ChoosePixelFormat(m_DeviceContext, &pfd);
	if (m_PixelFormat == 0) {
		LOG_ERROR(Renderer, "OpenGL-Context creation failed - no valid pixel format found.");
		return;
	}

	SetPixelFormat(m_DeviceContext, m_PixelFormat, &pfd);
	m_GLContext = wglCreateContext(m_DeviceContext);
}

Win32RenderContextGL::~Win32RenderContextGL()
{
	wglDeleteContext(m_GLContext);
	ReleaseDC((HWND)m_RefWindow->getNativeHandle(), m_DeviceContext);
}

void Win32RenderContextGL::_swapBuffer()
{
	SwapBuffers(m_DeviceContext);
}

void Win32RenderContextGL::_makeCurrent()
{
	wglMakeCurrent(m_DeviceContext, m_GLContext);
}

bool Win32RenderContextGL::_valid()
{
	return m_GLContext != nullptr;
}

//
/// NULL CONTEXT

Win32RenderContextNull::Win32RenderContextNull(Win32Window * contextForWindow)
{
}

Win32RenderContextNull::~Win32RenderContextNull()
{
}

void Win32RenderContextNull::_swapBuffer()
{
}

void Win32RenderContextNull::_makeCurrent()
{
}

bool Win32RenderContextNull::_valid()
{
	return true;
}

ENDNAMESPACE

#endif
