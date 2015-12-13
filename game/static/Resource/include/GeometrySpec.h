#pragma once

#include <Utilities/Flags.h>
#include <Utilities/UUID.h>

BEGINNAMESPACE

#define MAX_PER_16BIT 65535

DECLARE_EXCLUSIVE_FLAGS(PrimitiveType, Triangle, Quads, Line, Points);
DECLARE_EXCLUSIVE_FLAGS(BufferUsage, StaticDraw, DynamicDraw);
DECLARE_EXCLUSIVE_FLAGS(VertexAttrib, Position, Normal, Color, Texture, TangentBinormal, JointIndex, JointWeight);
DECLARE_EXCLUSIVE_FLAGS(VertexAttribType, Float32, Int32);
DECLARE_EXCLUSIVE_FLAGS(IndexType, _16Bit, _32Bit);

struct VertexLayoutSpec {
	static const uint32 MaxElementsPerLayout = 16;
	uint32 numberOfElements;
	uint32 elementType[MaxElementsPerLayout];
	uint8 elementCount[MaxElementsPerLayout];
};

struct GeometrySpec {
	static const uint32 MaxSubMeshes = 5;
	static const uint32 MaxVertexBuffer = 5;
	static const uint32 MaxTextureCoordChannel = 8;

	UUID uuid;


	//
	uint32 numberOfSubmeshes;
	uint32 numberOfColorChannel;
	uint32 numberOfTextureCoords;
	uint32 vertexAttribs;

	uint32 textureCoordComponentCount[MaxTextureCoordChannel];

	uint32 numberOfVertexBuffer;
	uint32 numberOfVerticesPerBuffer;
	uint32 bufferUsage;
	uint32 primitiveType;
	uint32 vertexStride[MaxVertexBuffer];

	VertexLayoutSpec vertexLayout[MaxVertexBuffer];

	struct {
		float32 minAABB[3];
		float32 maxAABB[3];
	} boundingVolume[MaxSubMeshes + 1];

	struct {
		uint32 startIndex;
		uint32 indexCount;
	} indices[MaxSubMeshes];

	uint32 vertexBufferSize;
	uint32 indexBufferSize;

	union {
		Byte* vertexData; //The vertex data is continious in memory
		float32* __vertexDataFloat32;
		UIntOfPtrSize __VertexDataLocation;
	};
	union {
		Byte* indexData;
		uint16* __indexDataUInt16;
		uint32* __indexDataUInt32;
		UIntOfPtrSize __IndexDataLocation;
	};
	static const GeometrySpec* FromBuffer(const Byte* buffer);
	static void ToBuffer(const GeometrySpec* res, Byte* buffer);
	static uint32 MemSize(const GeometrySpec* spec);
};

uint32 VertexStride(uint32 flags, uint32 numColorChannels, uint32 numUVChannels, uint32* channelComponents);

ENDNAMESPACE