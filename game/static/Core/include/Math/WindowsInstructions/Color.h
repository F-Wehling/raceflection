#pragma once

#include <Math/WindowsInstructions/__instructions.h>

BEGINNAMESPACE

namespace Colors = DirectX::Colors;

// some color functions
DX_NAMEALIAS(ColorSRGBToRGB);
DX_NAMEALIAS(ColorRGBToSRGB);
DX_NAMEALIAS(ColorAdjustContrast);
DX_NAMEALIAS(ColorAdjustSaturation);
DX_NAMEALIAS(ColorEqual);
DX_NAMEALIAS(ColorGreater);
DX_NAMEALIAS(ColorGreaterOrEqual);
DX_NAMEALIAS(ColorHSLToRGB);
DX_NAMEALIAS(ColorHSVToRGB);
DX_NAMEALIAS(ColorIsInfinite);
DX_NAMEALIAS(ColorIsNaN);
DX_NAMEALIAS(ColorLess);
DX_NAMEALIAS(ColorLessOrEqual);
DX_NAMEALIAS(ColorModulate);
DX_NAMEALIAS(ColorNegative);
DX_NAMEALIAS(ColorNotEqual);
DX_NAMEALIAS(ColorRGBToHSL);
DX_NAMEALIAS(ColorRGBToHSV);
DX_NAMEALIAS(ColorRGBToXYZ);
DX_NAMEALIAS(ColorRGBToYUV);
DX_NAMEALIAS(ColorRGBToYUV_HD);
DX_NAMEALIAS(ColorSRGBToXYZ);
DX_NAMEALIAS(ColorXYZToRGB);
DX_NAMEALIAS(ColorXYZToSRGB);
DX_NAMEALIAS(ColorYUVToRGB);
DX_NAMEALIAS(ColorYUVToRGB_HD);

ENDNAMESPACE