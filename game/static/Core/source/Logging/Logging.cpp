#include <Logging/Logging.h>
#include <Configuration/Configuration.h>

BEGINNAMESPACE

TConfigSettings<LogLevelFlags> g_Verbosity("verbosity", "Sets the logging verbosity (INFO, WARNING, DEBUG, ERROR; FATAL)", LogLevelFlags(LogLevel::Info | LogLevel::Warning | LogLevel::Debug | LogLevel::Error | LogLevel::Fatal));

GlobalLogger g_GlobalLogger;

ENDNAMESPACE