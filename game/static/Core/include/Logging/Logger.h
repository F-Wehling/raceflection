#pragma once

#include <Utilities/Flags.h>

#include <stdarg.h>

BEGINNAMESPACE


DECLARE_FLAGS(Channel, General, Renderer, Physic, Animation, Effect, Assertion);
DECLARE_FLAGS(LogLevel, Info, Warning, Debug, Error, Fatal, Assertion);

struct SourceInfo;

class Logger {
public:
	struct Criteria {
		ChannelFlags channel;
		LogLevelFlags type, verbosity;
		const SourceInfo& sourceInfo;
		const ansichar* format;
        va_list args;
        //va_list args;
	};
	static const size_type MAX_MESSAGE_LENGTH = 512;
	typedef ansichar Buffer[MAX_MESSAGE_LENGTH];
	virtual ~Logger();
	void log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, ...);
    virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) = 0;
};

ENDNAMESPACE
