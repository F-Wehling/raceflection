#pragma once

#include <Math/WindowsInstructions/__instructions.h>

BEGINNAMESPACE

//various conversions
DX_NAMEALIAS_PACKED(ConvertFloatToHalf);
DX_NAMEALIAS_PACKED(ConvertFloatToHalfStream);
DX_NAMEALIAS_PACKED(ConvertHalfToFloat);
DX_NAMEALIAS_PACKED(ConvertHalfToFloatStream);
DX_NAMEALIAS(ConvertToDegrees);
DX_NAMEALIAS(ConvertToRadians);
DX_NAMEALIAS(ConvertVectorFloatToInt);
DX_NAMEALIAS(ConvertVectorFloatToUInt);
DX_NAMEALIAS(ConvertVectorIntToFloat);
DX_NAMEALIAS(ConvertVectorUIntToFloat);

ENDNAMESPACE