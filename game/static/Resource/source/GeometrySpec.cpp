#include "GeometrySpec.h"

#include "AnimationSpec.h"

#include <numeric>

BEGINNAMESPACE

uint32 VertexStride(uint32 flags, uint32 numColorChannels, uint32 numUVChannels, uint32* channelComponents) {
	uint32 uvs = std::accumulate(channelComponents, channelComponents + numUVChannels, 0);
	uint32 stride = 0;
	stride += (flags & VertexAttrib::Position ? sizeof(float32) * 3 : 0);
	stride += (flags & VertexAttrib::Normal ? sizeof(float32) * 3 : 0);
	stride += (flags & VertexAttrib::Color ? sizeof(float32) * 4 * numColorChannels : 0);
	stride += (flags & VertexAttrib::Texture ? sizeof(float32) * uvs : 0);
	stride += (flags & VertexAttrib::TangentBinormal ? sizeof(float32) * 6 : 0);
	stride += (flags & VertexAttrib::JointIndex ? sizeof(uint32) * AnimationSpec::MaxJoints : 0);
	stride += (flags & VertexAttrib::JointWeight ? sizeof(float32) * AnimationSpec::MaxJoints : 0);

	return uint32(stride);
}

const GeometrySpec* GeometrySpec::FromBuffer(const Byte* _buffer) {
	union {
		const Byte* buffer;
		GeometrySpec* geometry;
		UIntOfPtrSize _geoLocation;
	};
	buffer = _buffer; //the buffer will contain the mesh-data

	uint32 sizeofIndex = geometry->numberOfVerticesPerBuffer * geometry->numberOfVertexBuffer <= 65535  ? sizeof(uint16) : sizeof(uint32);
	
	uint32 offset = uint32(sizeof(GeometrySpec));
	
	uint32 vertexStride = 0;
	vertexStride = std::accumulate(geometry->vertexStride, geometry->vertexStride + GeometrySpec::MaxSubMeshes, 0);

	geometry->__VertexDataLocation = _geoLocation + offset;
	offset += geometry->numberOfVertexBuffer * geometry->numberOfVerticesPerBuffer * vertexStride;

	geometry->__IndexDataLocation = _geoLocation + offset;

	return geometry;
}

void GeometrySpec::ToBuffer(const GeometrySpec * res, Byte * _buffer)
{
	union {
		GeometrySpec* spec;
		Byte* buffer;
	};
	size_type offset = 0;
	buffer = _buffer;
	*spec = *res;

	offset += sizeof(GeometrySpec);

	std::memcpy(buffer + offset, res->vertexData, res->vertexBufferSize);
	offset += res->vertexBufferSize;
	std::memcpy(buffer + offset, res->indexData, res->indexBufferSize);
}

uint32 GeometrySpec::MemSize(const GeometrySpec * spec)
{
	return sizeof(GeometrySpec) + spec->vertexBufferSize + spec->indexBufferSize;
}

ENDNAMESPACE