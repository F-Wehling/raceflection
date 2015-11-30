#pragma once

BEGINNAMESPACE

class Number {
public:
	static inline constexpr bool IsPowerOf2(size_type n) { return n != 0 && (n & (n - 1)) == 0; }
	static size_type RoundUp(size_type numToRound, size_type multipleOf);
	static size_type RoundUpToPowerOf2(size_type numToRound, size_type powerOf2);
	static size_type MaxWithNBit(size_type numBits);
	static size_type NextPowerOf2(size_type n);
};

ENDNAMESPACE