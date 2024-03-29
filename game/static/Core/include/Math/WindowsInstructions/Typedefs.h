#pragma once

#include <Math/WindowsInstructions/__instructions.h>

BEGINNAMESPACE

DX_TYPEDEF_PACKED(byte2, BYTE2);
DX_TYPEDEF_PACKED(byte4, BYTE4);
DX_TYPEDEF_PACKED(byteN2, BYTEN2);
DX_TYPEDEF_PACKED(byteN4, BYTEN4);
DX_TYPEDEF_PACKED(Color, COLOR);
DX_TYPEDEF_PACKED(dec4, DEC4);
DX_TYPEDEF_PACKED(decN4, DECN4);
DX_TYPEDEF(float2, FLOAT2);
DX_TYPEDEF(float3, FLOAT3);
DX_TYPEDEF(float4, FLOAT4);
DX_TYPEDEF(float2A, FLOAT2A);
DX_TYPEDEF(float3A, FLOAT3A);
DX_TYPEDEF(float4A, FLOAT4A);
DX_TYPEDEF_PACKED(float3PK, FLOAT3PK);
DX_TYPEDEF_PACKED(float3SE, FLOAT3SE);
DX_TYPEDEF(float3x3, FLOAT3X3);
DX_TYPEDEF(float4x3, FLOAT4X3);
DX_TYPEDEF(float4x3A, FLOAT4X3A);
DX_TYPEDEF(float4x4, FLOAT4X4);
DX_TYPEDEF(float4x4A, FLOAT4X4A);
DX_TYPEDEF_PACKED(half2, HALF2);
DX_TYPEDEF_PACKED(half4, HALF4);
DX_TYPEDEF(int2, INT2);
DX_TYPEDEF(int3, INT3);
DX_TYPEDEF(int4, INT4);
DX_TYPEDEF(Matrix, MATRIX);
DX_TYPEDEF_PACKED(short2, SHORT2);
DX_TYPEDEF_PACKED(short4, SHORT4);
DX_TYPEDEF_PACKED(shortN2, SHORTN2);
DX_TYPEDEF_PACKED(shortN4, SHORTN4);
DX_TYPEDEF_PACKED(u555, U555);
DX_TYPEDEF_PACKED(u565, U565);
DX_TYPEDEF_PACKED(ubyte2, UBYTE2);
DX_TYPEDEF_PACKED(ubyte4, UBYTE4);
DX_TYPEDEF_PACKED(ubyteN2, UBYTEN2);
DX_TYPEDEF_PACKED(ubyteN4, UBYTEN4);
DX_TYPEDEF_PACKED(udec4, UDEC4);
DX_TYPEDEF_PACKED(udecN4, UDECN4);
DX_TYPEDEF(uint2, INT2);
DX_TYPEDEF(uint3, INT3);
DX_TYPEDEF(uint4, INT4);
DX_TYPEDEF_PACKED(UNibble, UNIBBLE4);
DX_TYPEDEF_PACKED(ushort2, USHORT2);
DX_TYPEDEF_PACKED(ushort4, USHORT4);
DX_TYPEDEF_PACKED(ushortN2, USHORTN2);
DX_TYPEDEF_PACKED(ushortN4, USHORTN4);
DX_TYPEDEF_PACKED(xdec4, XDEC4);
DX_TYPEDEF_PACKED(xdecN4, XDECN4);

typedef DirectX::BoundingBox BoundingBox;
typedef DirectX::BoundingFrustum BoundingFrustum;
typedef DirectX::BoundingOrientedBox BoundingOrientedBox;
typedef DirectX::BoundingSphere BoundingSphere;
typedef float32 Scalar;

ENDNAMESPACE