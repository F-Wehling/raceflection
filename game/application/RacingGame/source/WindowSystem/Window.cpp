#include <WindowSystem/Window.h>

#include <Delegates/Delegate.h>

/*
#include <GLFW/glfw3.h>
#if OS_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32 1
#	define GLFW_EXPOSE_NATIVE_WGL 1
#elif OS_LINUX
#	define GLFW_EXPOSE_NATIVE_X11 1
#	define GLFW_EXPOSE_NATIVE_GLX 1
#endif
#include "GLFW/glfw3native.h"
//*/

#include "Logging/Logging.h"


BEGINNAMESPACE


Window::~Window()
{
}

/*
inline GLFWwindow* g(void* ptr) {
	return (GLFWwindow*)ptr;
}

bool Window::open()
{
	return open(WindowDesc::getDefault());
}

bool Window::open(WindowDesc desc)
{
	glfwDefaultWindowHints(); //reset window hints
	glfwWindowHint(GLFW_RESIZABLE, desc.resizable);
	glfwWindowHint(GLFW_VISIBLE, desc.visible);
	glfwWindowHint(GLFW_DECORATED, desc.decorated);
    //glfwWindowHint(GLFW_FOCUSED, desc.focused);
	//glfwWindowHint(GLFW_AUTO_ICONFIY, desc.autoMinimize);
	//glfwWindowHint(GLFW_FLOATING, desc.floating);
	glfwWindowHint(GLFW_RED_BITS, desc.red_bits);
	glfwWindowHint(GLFW_GREEN_BITS, desc.green_bits);
	glfwWindowHint(GLFW_BLUE_BITS, desc.blue_bits);
	glfwWindowHint(GLFW_ALPHA_BITS, desc.alpha_bits);
	glfwWindowHint(GLFW_DEPTH_BITS, desc.depth_bits);
	glfwWindowHint(GLFW_STENCIL_BITS, desc.stencil_bits);
	glfwWindowHint(GLFW_ACCUM_RED_BITS, desc.red_accum_bits);
	glfwWindowHint(GLFW_ACCUM_GREEN_BITS, desc.green_accum_bits);
	glfwWindowHint(GLFW_ACCUM_BLUE_BITS, desc.blue_accum_bits);
	glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, desc.alpha_accum_bits);
	glfwWindowHint(GLFW_AUX_BUFFERS, desc.aux_buffers);
	glfwWindowHint(GLFW_SAMPLES, desc.samples);
	glfwWindowHint(GLFW_REFRESH_RATE, desc.refresh_rate);
	glfwWindowHint(GLFW_STEREO, desc.stereo);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, desc.doublebuffer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc.major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc.minor_version);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, desc.forward_compat);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG_BUILD);

	if (!createWindow()) {
		return false;
	}
	setTitle(desc.title);
	setSize(desc.width, desc.height);
	setPosition(desc.posX, desc.posY);
	initializeCallbacks();
	return true;
}

bool Window::isClosed()
{
	return glfwWindowShouldClose(g(m_handle)) != 0;
}

void Window::setSize(int32 width, int32 height) {
	glfwSetWindowSize(g(m_handle), width, height);
}

void Window::setSize(const Size2i & size) {
	glfwSetWindowSize(g(m_handle), size.width, size.height);
}

Size2i Window::getSize() const
{
	Size2i e;
	glfwGetWindowSize(g(m_handle), &e.width, &e.height);
	return e;
}

void Window::getSize(int32 & width, int32 & height) const
{
	glfwGetWindowSize(g(m_handle), &width, &height);
}

Size2i Window::getFramebufferSize() const
{
	Size2i e;
	glfwGetFramebufferSize(g(m_handle), &e.width, &e.height);
	return e;
}

void Window::getFramebufferSize(int32 & width, int32 & height) const
{
	glfwGetFramebufferSize(g(m_handle),&width, &height);
}

void Window::setPosition(int32 xPos, int32 yPos)
{
	glfwSetWindowPos(g(m_handle), xPos, yPos);
}

void Window::setPosition(const Point2i& pos)
{
	glfwSetWindowPos(g(m_handle), pos.x, pos.y);
}

Point2i Window::getPosition() const
{
	Point2i p;
	glfwGetWindowPos(g(m_handle), &p.x, &p.y);
	return p;
}

void Window::getPosition(int32 & width, int32 & height) const
{
	glfwGetWindowPos(g(m_handle), &width, &height);
}

void Window::setTitle(const ansichar * title)
{
	glfwSetWindowTitle(g(m_handle), title);
}

void Window::minimize()
{
	glfwIconifyWindow(g(m_handle));
}

void Window::restore()
{
	glfwRestoreWindow(g(m_handle));
}

bool Window::isMinimized() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_ICONIFIED) != 0;
}

void Window::show()
{
	glfwShowWindow(g(m_handle));
}

void Window::hide()
{
	glfwHideWindow(g(m_handle));
}

bool Window::isVisible() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_VISIBLE) != 0;
}

bool Window::hasFocus() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_FOCUSED) != 0;
}

bool Window::isResizeable() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_RESIZABLE) != 0;
}

bool Window::isDecorated() const 
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_DECORATED) != 0;
}

void Window::getOpenGLContext(int32 & major, int32 & minor, int32 & revision) const
{
	major = glfwGetWindowAttrib(g(m_handle), GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(g(m_handle), GLFW_CONTEXT_VERSION_MINOR);
	revision = glfwGetWindowAttrib(g(m_handle), GLFW_CONTEXT_REVISION);
}

bool Window::isForwardCompatible() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_OPENGL_FORWARD_COMPAT) != GL_FALSE;
}

bool Window::isDebugContext() const
{
	return glfwGetWindowAttrib(g(m_handle), GLFW_OPENGL_DEBUG_CONTEXT) != GL_FALSE;
}

FramebufferDesc Window::getWindowFramebufferDesc() const
{
	FramebufferDesc fb_desc;
	glGetIntegerv(GL_RED_BITS, &fb_desc.red_bits);
	glGetIntegerv(GL_GREEN_BITS, &fb_desc.green_bits);
	glGetIntegerv(GL_BLUE_BITS, &fb_desc.blue_bits);
	glGetIntegerv(GL_ALPHA_BITS, &fb_desc.alpha_bits);
	glGetIntegerv(GL_DEPTH_BITS, &fb_desc.depth_bits);
	glGetIntegerv(GL_STENCIL_BITS, &fb_desc.stencil_bits);
	glGetIntegerv(GLFW_SAMPLES, &fb_desc.msaa_samples);
	return fb_desc;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(g(m_handle));
}

void Window::setSwapInterval(int32 interval)
{
	glfwSwapInterval(interval);
}

void Window::destroy()
{
	glfwDestroyWindow(g(m_handle));
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(g(m_handle));
}

void * Window::getNativeHandle()
{
#	if OS_WINDOWS
	return glfwGetWin32Window(g(m_handle));
#	elif OS_LINUX

    return (void*)glfwGetX11Window(g(m_handle));
#	endif
	return nullptr;
}

bool Window::createWindow()
{
	GLFWwindow* wnd = glfwCreateWindow(1, 1, "test", nullptr, nullptr);
	if (!wnd) {
		LOG_ERROR(General, "Window creation failed");
		return false;
	}
	glfwSetWindowUserPointer(wnd, (void*)this);
	m_handle = wnd;
	return true;
}

void Window::initializeCallbacks()
{
	glfwSetWindowCloseCallback(g(m_handle), (GLFWwindowclosefun)&Window::_CallCloseCallback);
	glfwSetWindowSizeCallback(g(m_handle), (GLFWwindowsizefun)&Window::_CallSizeCallback);
	glfwSetFramebufferSizeCallback(g(m_handle), (GLFWframebuffersizefun)&Window::_CallFramebufferSizeCallback);
	glfwSetWindowPosCallback(g(m_handle), (GLFWwindowposfun)&Window::_CallPositionChanged);
	glfwSetWindowFocusCallback(g(m_handle), (GLFWwindowfocusfun)&Window::_CallFocusChanged);
	glfwSetWindowIconifyCallback(g(m_handle), (GLFWwindowiconifyfun)&Window::_CallMinimizedRestored);
}

ENDNAMESPACE


BEGINNAMESPACE //call callbacks based on static fn

template<typename DelegatePtr, typename ...Args>
void callDelegate(void* ptr, DelegatePtr del, Args... args) {
	union {
		void* _ptr;
		GLFWwindow* _glfwWnd;
	};
	_ptr = ptr;
	Window* window = (Window*)glfwGetWindowUserPointer(_glfwWnd);
	Window::Delegates* delegates = window->getDelegates();
	if(delegates)(delegates->*del).invoke(window,std::forward<Args>(args)...);
}

void Window::_CallCloseCallback(void * ptr)
{
	union {
		void* _ptr;
		GLFWwindow* _glfwWnd;
	};
	_ptr = ptr;
	Window* window = (Window*)glfwGetWindowUserPointer(_glfwWnd);
	Window::Delegates* delegates = window->getDelegates();
	if (delegates) {
		delegates->onClose(window);
	}
	else {
		window->destroy();
	}	
}

void Window::_CallSizeCallback(void * ptr, int32 width, int32 height)
{
	callDelegate(ptr, &WindowDelegates::onResize, Size2i(width, height));
}

void Window::_CallFramebufferSizeCallback(void * ptr, int32 width, int32 height)
{
	callDelegate(ptr, &WindowDelegates::onFramebufferResize, Size2i(width, height));
}

void Window::_CallPositionChanged(void * ptr, int32 posX, int32 posY)
{
	callDelegate(ptr, &WindowDelegates::onPositionChanged, Point2i(posX, posY));
}

void Window::_CallMinimizedRestored(void * ptr, int32 minimized)
{
	callDelegate(ptr, &WindowDelegates::onMinimizeResized, minimized != 0);
}

void Window::_CallFocusChanged(void * ptr, int32 focus)
{
	callDelegate(ptr, &WindowDelegates::onFocusChanged, focus != 0);
}
*/

//Very simple default gl version
#if !ACGL_OPENGL_VERSION_42
#   define OPENGL_DEFAULT_MAJOR 3
#   define OPENGL_DEFAULT_MINRO 2
#else
#   define OPENGL_DEFAULT_MAJOR 4
#   define OPENGL_DEFAULT_MINRO 2
#endif
WindowDesc WindowDesc::getDefault(){
	WindowDesc desc;
	desc.title = "RACING - Default Window";
	desc.width = 1024;
	desc.height = 768;
	desc.posX = 200;
	desc.posY = 200;
	desc.accum_bits = 0;
	desc.buffer_bits = 0x11842108;
	desc.window_flags = 0x1F;
	desc.opengl_flags = 0x0227;
	desc.aux_buffers = 0;
	desc.refresh_rate = 0;
	desc.samples = 0;

    desc.major_version = OPENGL_DEFAULT_MAJOR;
    desc.minor_version = OPENGL_DEFAULT_MINRO;

	return desc;
}

ENDNAMESPACE
