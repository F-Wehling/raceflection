#pragma once

#include <Logging/Logger.h>
#include <CompileTime/Templates.h>
#include <CompileTime/Tuple.h>

#include <stdarg.h>

BEGINNAMESPACE

	template<typename FilterPolicy, typename FormatPolicy, typename WritePolicy>
	class LoggerImpl : public Logger{
	public:
        virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) override {
#           if OS_WINDOWS
            Criteria crit = { channel, type, verbosity, sourceInfo, format, args };
#           elif OS_LINUX
            Criteria crit = { channel, type, verbosity, sourceInfo, format};
            va_copy(crit.args, args);
#           endif
			if (m_filter.filter(crit)) { // check if it should be logged according to filter policy
				Buffer buffer;
				m_format.format(buffer, crit ); //format the message according to format policy
				m_writer.write(buffer); //write it based on writing policy
			}
		}
	protected:
	private:
		FilterPolicy m_filter;
		FormatPolicy m_format;
		WritePolicy m_writer;
	};

	template<typename ...LoggerList>
	class LoggerCombiner : public Logger {
		typedef Tuple<LoggerList...> TupleType;
	public:
        virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) override {
            for_each_on_tuple(logger, &Logger::_log, channel, type, verbosity, sourceInfo, format, (va_list&)args );
		}
	private:
		TupleType logger;
	};

    template<typename Log1>
    class LoggerCombiner<Log1> : public Logger {
    public:
        virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) override {
            logger._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
            //for_each_on_tuple(logger, &Logger::_log, channel, type, verbosity, sourceInfo, format, (va_list&)args );
        }
    private:
        Log1 logger;
    };

    template<typename Log1, typename Log2>
    class LoggerCombiner<Log1, Log2> : public Logger {
    public:
        virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) override {
            log1._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
            log2._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
        }
    private:
        Log1 log1;
        Log2 log2;
    };

    template<typename Log1, typename Log2, typename Log3>
    class LoggerCombiner<Log1, Log2, Log3> : public Logger {
    public:
        virtual void _log(ChannelFlags channel, LogLevelFlags type, LogLevelFlags verbosity, const SourceInfo& sourceInfo, const char* format, va_list& args) override {
            log1._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
            log2._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
            log3._log(channel, type, verbosity, sourceInfo, format, (va_list&)args);
        }
    private:
        Log1 log1;
        Log2 log2;
        Log3 log3;
    };

ENDNAMESPACE
