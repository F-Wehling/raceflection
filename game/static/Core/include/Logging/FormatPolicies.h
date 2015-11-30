#pragma once

#include <Logging/Logger.h>
#include <Utilities/SourceInfo.h>

#include <iostream>

BEGINNAMESPACE

namespace policy {

	struct SimpleFormat {
        void format(Logger::Buffer& buffer, Logger::Criteria& crit) {
            ansichar BUFF[512];
            std::vsnprintf(BUFF, sizeof(BUFF), crit.format, crit.args);
			std::snprintf(buffer, Logger::MAX_MESSAGE_LENGTH, "[%s]: %s", Channel::toString(crit.channel), BUFF);
		}
	};

	struct IdeFormat {
        void format(Logger::Buffer& buffer, Logger::Criteria& crit) {
			ansichar BUFF[512];
			std::vsnprintf(BUFF, 512, crit.format, crit.args);
			std::snprintf(buffer, Logger::MAX_MESSAGE_LENGTH, "%s(%d): [%s] (%s): %s %s\n", crit.sourceInfo.file, crit.sourceInfo.line, Channel::toString(crit.channel), LogLevel::toString(crit.type), crit.sourceInfo.function, BUFF);
		}
	};

}

ENDNAMESPACE
