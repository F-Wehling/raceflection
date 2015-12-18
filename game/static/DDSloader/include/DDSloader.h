#pragma once

#include "Utilities/Flags.h"

BEGINNAMESPACE

struct DDSAlphaMode {
	enum {
		Unknown = 0,
		Straight = 1,
		PreMultiplied = 2,
		Opaque = 3,
		Custom = 4
	};
};

class DDSloader {
public:
	bool textureFromMemory(const Byte* data, size_type dataSize);
};

ENDNAMESPACE