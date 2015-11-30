#if OS_WINDOWS
#include <Math/WindowsInstructions/Instructions.h>

BEGINNAMESPACE

//various conversions
DX_IMPL_NAMEALIAS_PACKED(ConvertFloatToHalf);
DX_IMPL_NAMEALIAS_PACKED(ConvertFloatToHalfStream);
DX_IMPL_NAMEALIAS_PACKED(ConvertHalfToFloat);
DX_IMPL_NAMEALIAS_PACKED(ConvertHalfToFloatStream);
DX_IMPL_NAMEALIAS(ConvertToDegrees);
DX_IMPL_NAMEALIAS(ConvertToRadians);
DX_IMPL_NAMEALIAS(ConvertVectorFloatToInt);
DX_IMPL_NAMEALIAS(ConvertVectorFloatToUInt);
DX_IMPL_NAMEALIAS(ConvertVectorIntToFloat);
DX_IMPL_NAMEALIAS(ConvertVectorUIntToFloat);

ENDNAMESPACE

#endif
