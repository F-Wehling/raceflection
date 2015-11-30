#include <Utilities/Assert.h>
#include <Configuration/Configuration.h>
#include <Logging/Logging.h>

#if OS_WINDOWS
#	include <Windows.h>
#endif

BEGINNAMESPACE

//ConfigSettingBool g_BreakOnAssert("g_BreakOnAssert", "Defines wheter an assert should trigger a breakpoint or not", true);

Assert::Assert(const SourceInfo& info, const ansichar* format, ...) {
	va_list args; va_start(args, format);
	LOG_VA(Assertion, Assertion, format, info, args);
    if (IsDebuggerConnected())
		DebugBreak();
}

bool IsDebuggerConnected() {
#	if OS_WINDOWS
	return IsDebuggerPresent() == TRUE;
#	else
#	endif
}

void DebugBreak() {
#	if OS_WINDOWS
	__debugbreak();
#	else
#	endif
}

ENDNAMESPACE
