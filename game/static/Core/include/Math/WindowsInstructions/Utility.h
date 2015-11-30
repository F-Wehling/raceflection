#pragma once

#include <Math/WindowsInstructions/__instructions.h>

BEGINNAMESPACE

// some utility functions
DX_NAMEALIAS(ComparisonAllFalse);
DX_NAMEALIAS(ComparisonAllInBounds);
DX_NAMEALIAS(ComparisonAllTrue);
DX_NAMEALIAS(ComparisonAnyFalse);
DX_NAMEALIAS(ComparisonAnyOutOfBounds);
DX_NAMEALIAS(ComparisonAnyTrue);
DX_NAMEALIAS(ComparisonMixed);
DX_NAMEALIAS(FresnelTerm);
DX_NAMEALIAS(VerifyCPUSupport);

namespace collision {
	using DirectX::TriangleTests::ContainedBy;
}

namespace intersection {
	using DirectX::TriangleTests::Intersects;
}
ENDNAMESPACE