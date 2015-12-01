#if OS_WINDOWS

#include "OS/Windows/Win32Window.h"
#include "OS/Windows/MinWindows.h"
#include "OS/Windows/Win32RenderContext.h"

//Control window by DWM rendering policies
#include <dwmapi.h>
#include <ShlObj.h>

#include "Logging.h"

BEGINNAMESPACE

extern const tchar* sAppClassName;

Win32Window::Win32Window() : Window() {

}

Win32Window::~Win32Window() {
}

bool Win32Window::_impl_open(WindowDesc desc)
{
	uint32 windowExStyle = 0;
	uint32 windowStyle = 0;

	m_HInstance = GetModuleHandle(0);

	// get some basic parameter
	int32 xPos = desc.posX;
	int32 yPos = desc.posY;
	size_type width = desc.width;
	size_type height = desc.height;

	if (!desc.decorated) {
		windowExStyle |= WS_EX_WINDOWEDGE;

		/*
		switch (m_Definition->transparencySupport) {
		case WindowTransparency::PerWindow:
			windowExStyle |= WS_EX_LAYERED;
			break;
		case WindowTransparency::PerPixel:
			windowExStyle |= WS_EX_COMPOSITED;
		}
		//*/

		windowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		if (desc.taskbar) {
			windowExStyle |= WS_EX_APPWINDOW;
		}
		else {
			windowExStyle |= WS_EX_TOOLWINDOW;
		}

		/*
		if (m_Definition->isTopmost)
		{
			windowExStyle |= WS_EX_TOPMOST;
		}

		if (!m_Definition->acceptsInput)
		{
			windowExStyle |= WS_EX_TRANSPARENT;
		}
		//*/
	}
	else {
		windowExStyle = WS_EX_APPWINDOW;
		windowStyle = WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION;

		if (desc.decorated)
		{
			windowStyle |= WS_MAXIMIZEBOX;
			windowStyle |= WS_MINIMIZEBOX;
		}

		// Note SizeX and SizeY should be the size of the client area.  We need to get the actual window size by adjusting the client size to account for standard windows border around the window
		RECT WindowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
		::AdjustWindowRectEx(&WindowRect, windowStyle, 0, windowExStyle);

		xPos += WindowRect.left;
		yPos += WindowRect.top;
		width = WindowRect.right - WindowRect.left;
		height = WindowRect.bottom - WindowRect.top;
	}
	
	m_Handle = CreateWindowEx(
		windowExStyle,
		sAppClassName,
		desc.title,
		windowStyle,
		xPos, yPos, width, height,
		nullptr, //(parent ? static_cast<HWND>(parent->m_HWND) : nullptr),
		0, m_HInstance, 0);


	if (!m_Handle) {
		LOG_ERROR(General, "WindowsWindow::initialize Creation Failed: %d", GetLastError());
		return false;
	}

	_impl_setSize(width, height);
	_impl_setPosition(xPos, yPos);

	SetWindowLongPtr(m_Handle, GWLP_USERDATA, (LONG_PTR) this);

	/*
	if (m_Definition->transparencySupport == WindowTransparency::PerWindow) {
		setOpacity(m_Definition->opacity);
	}
	*/

	if (!desc.decorated) {
		const DWMNCRENDERINGPOLICY renderingPolicy = DWMNCRP_DISABLED;
		DwmSetWindowAttribute(m_Handle, DWMWA_NCRENDERING_POLICY, &renderingPolicy, sizeof(DWMNCRENDERINGPOLICY));

		const bool enableNCPaint = false;
		DwmSetWindowAttribute(m_Handle, DWMWA_ALLOW_NCPAINT, &enableNCPaint, sizeof(bool));

		/*
		if (m_Definition->transparencySupport == WindowTransparency::PerPixel) {
			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(m_HWND, &margins);
		}
		*/

		windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		
		windowStyle |= WS_MAXIMIZEBOX;
		windowStyle |= WS_MINIMIZEBOX;
		
		if (desc.resizable)
		{
			windowStyle |= WS_THICKFRAME;
		}

		SetWindowLong(m_Handle, GWL_STYLE, windowStyle);
		SetWindowPos(m_Handle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		adjustWindowRegion(width, height);
	}

	/* ignore drag drop
	if (isRegularWindow()) {
		RegisterDragDrop(m_HWND, this);
	}
	//*/

	ShowWindow(m_Handle, SW_SHOW);

	return true;
}

bool Win32Window::_impl_isClosed()
{
	return IsWindow(m_Handle) != TRUE;
}

void Win32Window::_impl_setSize(int32 width, int32 height)
{
	WINDOWINFO windowInfo;
	ZeroMemory(&windowInfo, sizeof(WINDOWINFO));
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_Handle, &windowInfo);


	RECT clientRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	if (m_Desc.decorated) {
		AdjustWindowRectEx(&windowRect, windowInfo.dwStyle, 0, windowInfo.dwExStyle);

		width = windowRect.right - windowRect.left;
		height = windowRect.bottom - windowRect.top;
	}

	SetWindowPos(m_Handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_WindowMode == WindowMode::Fullscreen) ? SWP_NOSENDCHANGING : 0));
	m_Width = width;
	m_Height = height;
}

void Win32Window::_impl_getSize(int32 & width, int32 & height) const
{
	width = m_Width; 
	height = m_Height;
}

void Win32Window::_impl_setPosition(int32 xPos, int32 yPos)
{
	RECT windowRect;
	GetWindowRect(m_Handle, &windowRect);

	POINT clientPoint;
	clientPoint.x = 0;
	clientPoint.y = 0;
	ClientToScreen(m_Handle, &clientPoint);

	const int32 xMoveDistance = xPos - clientPoint.x;
	const int32 yMoveDistance = yPos - clientPoint.y;

	int32 X = windowRect.left + xMoveDistance;
	int32 Y = windowRect.top + yMoveDistance;

	MoveWindow(m_Handle, X, Y, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, true);
	m_XPos = X;
	m_YPos = Y;
}

void Win32Window::_impl_getPosition(int32 & xpos, int32 & ypos) const
{
	xpos = m_XPos;
	ypos = m_YPos;
}

void Win32Window::_impl_getFramebufferSize(int32 & width, int32 & height) const
{
	width = m_Width;
	height = m_Height;
}

void Win32Window::_impl_setTitle(const ansichar * title)
{
	SetWindowText(m_Handle, title);
}

void Win32Window::_impl_minimize()
{
	ShowWindow(m_Handle, SW_MINIMIZE);
}

void Win32Window::_impl_maximize()
{
	ShowWindow(m_Handle, SW_MAXIMIZE);
}

void Win32Window::_impl_restore()
{
	ShowWindow(m_Handle, SW_RESTORE);
}

bool Win32Window::_impl_isMinimized() const
{
	return IsIconic(m_Handle) == TRUE;
}

bool Win32Window::_impl_isMaximized() const
{
	return IsZoomed(m_Handle) == TRUE;
}

void Win32Window::_impl_show()
{
	ShowWindow(m_Handle, SW_SHOW);
}

void Win32Window::_impl_hide()
{
	ShowWindow(m_Handle, SW_HIDE);
}

bool Win32Window::_impl_isVisible() const
{
	return IsWindowVisible(m_Handle) == TRUE;
}

bool Win32Window::_impl_hasFocus() const
{
	return false;
}

bool Win32Window::_impl_isResizeable() const
{
	return m_Desc.resizable;
}

bool Win32Window::_impl_isDecorated() const
{
	return m_Desc.decorated;
}

void Win32Window::_impl_getOpenGLContext(int32 & major, int32 & minor, int32 & revision) const
{
}

bool Win32Window::_impl_isForwardCompatible() const
{
	return false;
}

bool Win32Window::_impl_isDebugContext() const
{
	return false;
}

FramebufferDesc Win32Window::_impl_getWindowFramebufferDesc() const
{
	return FramebufferDesc();
}

void Win32Window::_impl_swapBuffers()
{
	if (m_RenderContext) m_RenderContext->swapBuffer();
}

void Win32Window::_impl_destroy()
{
	DestroyWindow(m_Handle);
}

void * Win32Window::_impl_getNativeHandle()
{
	return (void*)m_Handle;
}

bool Win32Window::_impl_createContext(ContextTypeFlags contextType)
{
	static_assert(sizeof(Win32RenderGLContext) <= sizeof(ContextStorage), "The render-context storage is too small");
	switch (contextType) {
	case ContextType::OpenGL:
		m_RenderContext = new (__contextStorage)Win32RenderGLContext(this);
		if (m_RenderContext->valid()) return true;
		m_RenderContext->~RenderContext();//free
		break;
	default: 
		LOG_ERROR(Renderer, "No context for %s available.", ContextType::toString(contextType));
		return false;
	}
	return false;
}

size_type Win32Window::getBorderSize() const
{
	WINDOWINFO windowInfo;
	ZeroMemory(&windowInfo, sizeof(WINDOWINFO));
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_Handle, &windowInfo);
	return windowInfo.cxWindowBorders;
}

HRGN Win32Window::makeWindowRegionObject(int32 width, int32 height) const
{
	HRGN reg;
	
	if (width > 0 || height > 0) {
		if (_impl_isMaximized()) {
			size_type borderSize = getBorderSize();
			reg = CreateRectRgn(borderSize, borderSize, width - borderSize, height - borderSize);
		}
		else {
			reg = CreateRectRgn(0, 0, width, height);
		}
	}
	else {
		RECT rcWnd;
		GetWindowRect(m_Handle, &rcWnd);
		reg = CreateRectRgn(0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
	}
	return reg;
}

void Win32Window::adjustWindowRegion(int32 width, int32 height)
{
	HRGN reg = makeWindowRegionObject(width, height);
	SetWindowRgn(m_Handle, reg, false);
}

//
/// message queue
int32 Win32Window::processMessage(HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	Win32Window* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (!window) return DefWindowProc(hWnd, msg, wParam, lParam); // return if window-ptr is 0

	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		if (window->m_delegates) window->m_delegates->onFocusChanged;
		if (window->m_delegates) window->m_delegates->onMinimizeResized;
		if (window->m_delegates) window->m_delegates->onPositionChanged;
		break;
	case WM_ACTIVATE:
		return 0;
	case WM_ACTIVATEAPP:
		break;
	case WM_NCACTIVATE:
		break;
	case WM_NCPAINT:
		break;
	case WM_CLOSE:
		if (window->m_delegates) window->m_delegates->onClose.invoke(window);
		break;
	case WM_SHOWWINDOW:
		/*
		switch (lParam) {
		case SW_PARENTCLOSING: window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentMinimize)); break;
		case SW_PARENTOPENING: window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentRestore)); break;
		default:;
		}
		switch (wParam) {
		case TRUE: window->listener.signal(window, WindowEvent::Generate(WindowEvent::Show, true));; break;
		case FALSE: window->listener.signal(window, WindowEvent::Generate(WindowEvent::Close, false)); break;
		default:;
		}
		//*/
		break;
	case WM_SIZE:
	{
		/*
		switch (wParam) {
		case SIZE_MAXHIDE:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::ParentMaximize));
			break;
		case SIZE_MAXIMIZED:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::Maximize));
			break;
		case SIZE_MINIMIZED:
			window->listener.signal(window, WindowEvent::Generate(WindowEvent::Minimize));
			break;
		default:;
		}
		*/
		int32 newWidth = (int32)(int16)(LOWORD(lParam));
		int32 newHeight = (int32)(int16)(HIWORD(lParam));
		WindowDesc desc = window->getWindowDesc();
		if (!desc.decorated) {
			window->adjustWindowRegion(newWidth, newHeight);
		}
		if (window->m_delegates) window->m_delegates->onResize(window, Size2i(newWidth, newHeight));
	}break;
	case WM_MOVE:
	{
		int32 newX = (int32)(int16)(LOWORD(lParam));
		int32 newY = (int32)(int16)(HIWORD(lParam));
		if (window->m_delegates) window->m_delegates->onPositionChanged(window, Point2i(newX, newY));
	}
	case WM_SIZING:
		break;
	default:;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


ENDNAMESPACE

#endif