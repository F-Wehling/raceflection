#include "WindowSystem/WindowSystem.h"

#include "RenderSystem/RenderContext.h"

#include "Logging.h"

BEGINNAMESPACE


WindowSystem::WindowSystem() : m_Allocator("WindowSystem", sizeof(PlatformWindow) * WindowDesc::MaxWindows)
{
	m_Allocator.initialize(sizeof(PlatformWindow), alignof(PlatformWindow));
}

bool WindowSystem::initialize()
{
	return true;
}

void WindowSystem::shutdown() {
}

bool pumpMessages(); //defined by the OS-layer
bool WindowSystem::tick(float32 dt)
{
    return pumpMessages();
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
