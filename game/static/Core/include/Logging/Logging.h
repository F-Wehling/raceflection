#pragma once

//#include <easylogging++.h>

#include <Logging/Logger.h>
#include <Logging/LoggerImpl.h>
#include <Logging/FilterPolicies.h>
#include <Logging/FormatPolicies.h>
#include <Logging/WriterPolicies.h>

#include <Utilities/SourceInfo.h>
#include <Configuration/Configuration.h>

BEGINNAMESPACE

//typedefs for logger
typedef policy::SpecificLevel< uint32(LogLevel::Error), uint32(LogLevel::Fatal) > IdeLogLevel; //Only log error and fatal on the ide
typedef LoggerImpl<IdeLogLevel, policy::IdeFormat, policy::IdeWriter> IDELogger;
typedef LoggerImpl<policy::NoFilter, policy::SimpleFormat, policy::ConsoleWriter> ConsoleLogger;

//combine them to a Global Logger
typedef LoggerCombiner<IDELogger, ConsoleLogger> GlobalLogger;

extern TConfigSettings<LogLevelFlags> g_Verbosity; //Set the verbosity level of the logger
extern GlobalLogger g_GlobalLogger; //the global logger as defined above


#if DISABLE_DEBUG
#	define LOG(channel, level, format, ...)
#	define LOG_INFO(channel, format, ...)
#	define LOG_ERROR(channel, format, ...)
#	define LOG_WARNING(channel, format, ...)
#	define LOG_DEBUG(channel, format, ...)
#	define LOG_FATAL(channel, format, ...)
#else
#   if OS_LINUX
#       define LOG(channel, level, format, ...) g_GlobalLogger.log(Channel::channel, LogLevel::level, g_Verbosity, SOURCE_INFO, format, ##__VA_ARGS__)
#       define LOG_INFO(channel, format, ...) LOG(channel, Info, format, ##__VA_ARGS__)
#       define LOG_ERROR(channel, format, ...) LOG(channel, Error, format, ##__VA_ARGS__)
#       define LOG_WARNING(channel, format, ...) LOG(channel, Warning, format, ##__VA_ARGS__)
#       define LOG_DEBUG(channel, format, ...) LOG(channel, Debug, format, ##__VA_ARGS__)
#       define LOG_FATAL(channel, format, ...) LOG(channel, Fatal, format, ##__VA_ARGS__)
#   else
#       define LOG(channel, level, format, ...) g_GlobalLogger.log(Channel::channel, LogLevel::level, g_Verbosity, SOURCE_INFO, format, __VA_ARGS__)
#       define LOG_INFO(channel, format, ...) LOG(channel, Info, format, __VA_ARGS__)
#       define LOG_ERROR(channel, format, ...) LOG(channel, Error, format, __VA_ARGS__)
#       define LOG_WARNING(channel, format, ...) LOG(channel, Warning, format, __VA_ARGS__)
#       define LOG_DEBUG(channel, format, ...) LOG(channel, Debug, format, __VA_ARGS__)
#       define LOG_FATAL(channel, format, ...) LOG(channel, Fatal, format, __VA_ARGS__)
#   endif
#	define LOG_VA(channel, level, format, sourceInfo, va_args) g_GlobalLogger._log(Channel::channel, LogLevel::level, g_Verbosity, sourceInfo, format, va_args)
#endif

ENDNAMESPACE
