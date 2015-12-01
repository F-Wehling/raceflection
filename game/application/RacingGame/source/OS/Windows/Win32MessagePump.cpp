#pragma once
#if OS_WINDOWS

#include <Windows.h>

BEGINNAMESPACE

bool pumpMessages() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

ENDNAMESPACE

#endif