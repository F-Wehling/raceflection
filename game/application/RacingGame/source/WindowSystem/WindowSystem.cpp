#include <WindowSystem/WindowSystem.h>

#include <RenderSystem/RenderContext.h>

#include <Logging/Logging.h>

BEGINNAMESPACE

Byte windowMemory[WindowSystem::sSystemMemorySize];

WindowSystem::WindowSystem() : m_Allocator("WindowSystem")
{
	m_Allocator.initialize(sizeof(PlatformWindow), alignof(PlatformWindow), windowMemory, sSystemMemorySize, 0);

}

bool WindowSystem::initialize()
{
	/*
	if (!glfwInit()) {
		LOG_ERROR(General, "GLFW initialization failed.");
		return false;
	}

	glfwSetErrorCallback((GLFWerrorfun)&WindowSystem::error_callback);
	//*/
	return true;
}

void WindowSystem::shutdown() {
	//glfwTerminate();
}

bool WindowSystem::tick(float32 dt)
{
#	if OS_WINDOWS
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#	elif OS_LINUX

#	endif
	
	return true;
}

Window * WindowSystem::openWindow()
{
	Window* window = eng_new(PlatformWindow, m_Allocator); //get a new window
	if (!window) {
		LOG_ERROR(General, "Can't create new memory instance. Exceeded maximum window limit");
		return nullptr;
	}
	if (!window->open()) {
		eng_delete(window, m_Allocator); //free the instance if opening failed
		return nullptr;
	}
	return window;
}

Window * WindowSystem::openWindow(WindowDesc desc)
{
	Window* window = eng_new(PlatformWindow, m_Allocator); //get a new window
	if (!window) {
		LOG_ERROR(General, "Can't create new memory instance. Exceeded maximum window limit");
		return nullptr;
	}
	if (!window->open(desc)) {
		eng_delete(window, m_Allocator); //free the instance if opening failed
		return nullptr;
	}
	return window;
}

void WindowSystem::destroyWindow(Window * window)
{
	window->destroy();
	eng_delete(window, m_Allocator);
}

void WindowSystem::drawTo(Window * window)
{
	if (window->getRenderContext()) window->getRenderContext()->makeCurrent();
}

void WindowSystem::error_callback(int32 error, const ansichar * description)
{
	LOG_ERROR(General, "WindowSystem Error %d: %s", error, description);
}


ENDNAMESPACE