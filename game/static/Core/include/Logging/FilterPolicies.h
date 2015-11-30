#pragma once

#include <Logging/Logger.h>

BEGINNAMESPACE

namespace policy {

	struct NoFilter {
		inline bool filter(const Logger::Criteria&) { return true; }
	};

	struct NoLogging {
		inline bool filter(const Logger::Criteria&) { return false; }
	};

	struct VerbosityFilter {
		inline bool filter(const Logger::Criteria& crit) {
			return crit.type < crit.verbosity;
		}
	};

	struct ChannelFilter {
		inline bool filter(const Logger::Criteria& crit) {
			return crit.channel & channel;
		}
		ChannelFlags channel;
	};

    template<uint32 ...lvl>
	struct SpecificLevel {			
        static const size_type numLevels = sizeof...(lvl);
		
		template<uint32 first, uint32 ...remain>
		struct contained {
			inline static bool level(uint32 flag) {
				return ((first & flag) != 0) || contained<remain...>::level(flag);
			}
		};

		template<uint32 first>
		struct contained<first> {
			inline static bool level(uint32 flag) {
				return (first & flag) != 0;
			}
		};

		inline bool filter(const Logger::Criteria& crit) {
			uint32 e = crit.type;
            return contained<lvl...>::level(e);
		}
	};
}

ENDNAMESPACE
