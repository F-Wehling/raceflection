#include "EffectSpec.h"

BEGINNAMESPACE

const EffectSpec* EffectSpec::FromBuffer(const Byte* _buffer) {
	union {
		const Byte* buffer;
		EffectSpec* spec;
		UIntOfPtrSize location;
	};
	buffer = _buffer;
	spec->__effectSourceLocation = location + sizeof(EffectSpec);
	return spec;
}

void EffectSpec::ToBuffer(const EffectSpec* res, Byte* buffer) {
	*(EffectSpec*)buffer = *res;
	std::memcpy(buffer + sizeof(EffectSpec), res->effectSource, (std::strlen(res->effectSource) + 1) * sizeof(ansichar));
}

uint32 EffectSpec::MemSize(const EffectSpec* spec) {
	return sizeof(EffectSpec) + (std::strlen(spec->effectSource) + 1) * sizeof(ansichar);
}

ENDNAMESPACE