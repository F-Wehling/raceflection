#pragma once

//SIMD-based Vector math is platform specific, so include it here
#if OS_WINDOWS
#	include <Math/WindowsInstructions/Instructions.h>
#elif OS_LINUX

#endif

BEGINNAMESPACE

namespace intersection {
	static const size_type Front = 0;
	static const size_type Intersecting = 1;
	static const size_type Back = 2;
}

namespace collision {
	static const size_type Disjoint = 0;
	static const size_type Intersects = 1;
	static const size_type CONTAINS = 2;
}

struct Constants {
	static const float32 c_Pi;
	static const float32 c_2Pi;
	static const float32 c_1DivPi;
	static const float32 c_1Div2Pi;
	static const float32 c_PiDiv2;
	static const float32 c_PiDiv4;
};

ENDNAMESPACE