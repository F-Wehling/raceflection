#pragma once

#include <Utilities/Flags.h>
#include <Utilities/UUID.h>

BEGINNAMESPACE

#define MAX_PER_16BIT 65535

DECLARE_FLAGS(GameObjectFlags, TriggerArea, Physics, Mesh);

struct GameObjectSpec {
	UUID uuid;

    UUID meshId;
    UUID physicsId;

    uint32 flags;

    float32 position[3];
    float32 rotation[4];
    float32 scaling[3];

    static const GameObjectSpec* FromBuffer(const Byte* buffer);
    static void ToBuffer(const GameObjectSpec* res, Byte* buffer);
    static uint32 MemSize(const GameObjectSpec* spec);
};

ENDNAMESPACE
