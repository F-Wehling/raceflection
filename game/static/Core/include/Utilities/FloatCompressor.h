#pragma once

BEGINNAMESPACE

class Float16Compressor
{
	union Bits {
		float32 f;
		int32 si;
		uint32 ui;
	};

	static const int32 shift = 13;
	static const int32 shiftSign = 16;

	static const int32 infN = 0x7F800000; // flt32 infinity
	static const int32 maxN = 0x477FE000; // max flt16 normal as a flt32
	static const int32 minN = 0x38800000; // min flt16 normal as a flt32
	static const int32 signN = 0x80000000; // flt32 sign bit

	static const int32 infC = infN >> shift;
	static const int32 nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
	static const int32 maxC = maxN >> shift;
	static const int32 minC = minN >> shift;
	static const int32 signC = signN >> shiftSign; // flt16 sign bit

	static const int32 mulN = 0x52000000; // (1 << 23) / minN
	static const int32 mulC = 0x33800000; // minN / (1 << (23 - shift))

	static const int32 subC = 0x003FF; // max flt32 subnormal down shifted
	static const int32 norC = 0x00400; // min flt32 normal down shifted

	static const int32 maxD = infC - maxC - 1;
	static const int32 minD = minC - subC - 1;

public:

	static uint16 compress(float32 value);

	static float32 decompress(uint16 value);
};

ENDNAMESPACE