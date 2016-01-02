#pragma once

#include "Utilities/UUID.h"
#include "Utilities/Flags.h"

BEGINNAMESPACE

DECLARE_EXCLUSIVE_FLAGS(EffectType, MATERIAL_FX, SCENE_FX, FXHeader);

struct EffectSpec {
	static const uint32 MaxEffectName = 255;
	ansichar name[MaxEffectName]; //name is necessary to resolve "include" commands ingame

	uint32 effectType; //whether it is a effect or a effect header

	UUID uuid;

	union {
		ansichar* effectSource; //The source code of the effect
		UIntOfPtrSize __effectSourceLocation;
	};
	static const EffectSpec* FromBuffer(const Byte* buffer);
	static void ToBuffer(const EffectSpec* res, Byte* buffer);
	static uint32 MemSize(const EffectSpec* spec);
};

ENDNAMESPACE