#pragma once

#include <WindowSystem/WindowDesc.h>
#include <WindowSystem/Window.h>
#include <MemorySystem.h>

BEGINNAMESPACE

class Window;

class WindowSystem {
	
public:
	WindowSystem();

	bool initialize();
	void shutdown();

	bool tick(float32 dt);

	Window* openWindow();
	Window* openWindow(WindowDesc desc);

	void destroyWindow(Window* window);

	void drawTo(Window* window);
private:
	static void error_callback(int32 error, const ansichar* description);

private: //The memory for this system
	typedef ProxyAllocator<PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> WindowSystemAllocator;
	WindowSystemAllocator m_Allocator;
};

ENDNAMESPACE