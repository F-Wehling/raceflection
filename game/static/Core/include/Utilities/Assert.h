#pragma once

#include <Utilities/SourceInfo.h>
#include <CompileTime/Preprocessor.h>

BEGINNAMESPACE


bool IsDebuggerConnected(); //Checks wheter the debugger is available
void DebugBreak(); //debugger break

class Assert {
public:
	Assert(const SourceInfo& info, const ansichar* format, ...);
};

#if RETAIL_BUILD
#	define ASSERT(condition, format, ...)
#elif OS_LINUX
#	define ASSERT(condition, format, ...)    if(!(condition)) Assert(SOURCE_INFO, "Assertion \"" #condition "\" failed. " format, ##__VA_ARGS__)
#else
#	define ASSERT(condition, format, ...)    (condition) ? NO_OP() : Assert(SOURCE_INFO, "Assertion \"" #condition "\" failed. " format, __VA_ARGS__)
#endif

ENDNAMESPACE
