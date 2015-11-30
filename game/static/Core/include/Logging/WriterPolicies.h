#pragma once

#include <Logging/Logger.h>

#if OS_WINDOWS
#	include <Windows.h>
#endif

#include <iostream>

BEGINNAMESPACE

namespace policy {

	struct IdeWriter {
		inline void write(const Logger::Buffer& buffer) {
#		if OS_WINDOWS
			OutputDebugStringA(buffer);
#		elif OS_LINUX
			
#		endif
		}
	};

	struct ConsoleWriter {
		inline void write(const Logger::Buffer& buffer) {
			std::cout << buffer << std::endl;
		}
	};
}

ENDNAMESPACE