#pragma once

#include <Utilities/Flags.h>
#include <Utilities/UUID.h>

BEGINNAMESPACE

#define MAX_PER_16BIT 65535

DECLARE_EXCLUSIVE_FLAGS(CollisionType, Sphere, Box, CylinderX, CylinderY, CylinderZ, Plane);

struct PhysicsSpec {
	UUID uuid;

    float32 mass;
    float32 restitution;

    uint32 collisionType;

	union {
        Byte* collisionShapeData;
        float32* collisionShapeDataFloat32;
        UIntOfPtrSize collisionShapeDataLocation;
    };
    static const PhysicsSpec* FromBuffer(const Byte* buffer);
    static void ToBuffer(const PhysicsSpec* res, Byte* buffer);
    static uint32 MemSize(const PhysicsSpec* spec);
};

ENDNAMESPACE
