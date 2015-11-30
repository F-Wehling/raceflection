#include <Logging/Logger.h>

#include <stdarg.h>

BEGINNAMESPACE

Logger::~Logger(){}

void Logger::log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, ...) {
	va_list args;
	va_start(args, format);
	_log(channel, type, verbosity, sourceInfo, format, args);
}


ENDNAMESPACE