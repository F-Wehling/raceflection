#include "PhysicsSpec.h"

#include <numeric>

BEGINNAMESPACE

const PhysicsSpec* PhysicsSpec::FromBuffer(const Byte* _buffer) {
	union {
		const Byte* buffer;
        PhysicsSpec* physics;
        UIntOfPtrSize _phyLocation;
	};
    buffer = _buffer; //the buffer will contain the mesh-data
	
    uint32 offset = uint32(sizeof(PhysicsSpec));

    physics->collisionShapeDataLocation = _phyLocation + offset;

    return physics;
}

void PhysicsSpec::ToBuffer(const PhysicsSpec * res, Byte * _buffer)
{
	union {
        PhysicsSpec* spec;
		Byte* buffer;
	};
	size_type offset = 0;
	buffer = _buffer;
	*spec = *res;

    offset += sizeof(PhysicsSpec);

    uint32 dataSize = 0;
    switch(spec->collisionType){
    case CollisionType::Box:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::Sphere:
        dataSize = sizeof(float32);
        break;
    case CollisionType::Plane:
        dataSize = 4 * sizeof(float32);
        break;
    case CollisionType::CylinderX:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::CylinderY:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::CylinderZ:
        dataSize = 3 * sizeof(float32);
        break;
    }
    std::memcpy(buffer + offset, res->collisionShapeData, dataSize);
}

uint32 PhysicsSpec::MemSize(const PhysicsSpec * spec)
{
    uint32 dataSize = 0;
    switch(spec->collisionType){
    case CollisionType::Box:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::Sphere:
        dataSize = sizeof(float32);
        break;
    case CollisionType::Plane:
        dataSize = 4 * sizeof(float32);
        break;
    case CollisionType::CylinderX:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::CylinderY:
        dataSize = 3 * sizeof(float32);
        break;
    case CollisionType::CylinderZ:
        dataSize = 3 * sizeof(float32);
        break;
    }
    return sizeof(PhysicsSpec) + dataSize;
}

ENDNAMESPACE
