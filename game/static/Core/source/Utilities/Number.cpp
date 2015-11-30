#include <Utilities/Number.h>

BEGINNAMESPACE

size_type Number::RoundUp(size_type numberToRound, size_type multipleOf) {
	if (multipleOf == 0) return numberToRound;

	size_type remainder = numberToRound % multipleOf;
	if (remainder == 0) return numberToRound;
	return numberToRound + multipleOf - remainder;
}

size_type Number::RoundUpToPowerOf2(size_type numToRound, size_type powerOf2) {
	ASSERT(IsPowerOf2(powerOf2), "RoundUpToMultipleOf2: powerOf2 isn't a power of 2");
	return (numToRound + powerOf2 - 1) & ~(powerOf2 - 1);
}

size_type Number::MaxWithNBit(size_type numBits) {
	static const size_type MAX = std::numeric_limits<size_type>::max();
	return (MAX >> (sizeof(size_type) * 8 - numBits));
}

size_type Number::NextPowerOf2(size_type n)
{
	if (n < 1) return 0;
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	return n + 1;
}

ENDNAMESPACE