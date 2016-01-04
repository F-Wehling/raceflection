#include "PackageSpec.h"
#include "ResourceSpec.h"

BEGINNAMESPACE

PackageSpec::PackageSpec(PackageAllocator* alloc) :
	m_PkgAllocator(alloc),
	m_Animations(nullptr),
	m_Audios(nullptr),
	m_Effects(nullptr),
	m_Geometries(nullptr),
	m_Lights(nullptr),
	m_Materials(nullptr),
	m_Meshs(nullptr),
	m_Physics(nullptr),
	m_Textures(nullptr),
	m_numberOfAnimation(0),
	m_numberOfAudio(0),
	m_numberOfEffects(0),
	m_numberOfGeometry(0),
	m_numberOfLight(0),
	m_numberOfMaterial(0),
	m_numberOfMesh(0),
	m_numberOfResources(0),
	m_numberOfPhysics(0),
	m_numberOfTextures(0)
{}

PackageSpec::~PackageSpec() {
	eng_delete(m_Animations, *m_PkgAllocator);
	eng_delete(m_Audios, *m_PkgAllocator);
	eng_delete(m_Effects, *m_PkgAllocator);
	eng_delete(m_Geometries, *m_PkgAllocator);
	eng_delete(m_Lights, *m_PkgAllocator);
	eng_delete(m_Materials, *m_PkgAllocator);
	eng_delete(m_Meshs, *m_PkgAllocator);
	eng_delete(m_Physics, *m_PkgAllocator);
	eng_delete(m_Textures, *m_PkgAllocator);
	m_numberOfAnimation = 0;
	m_numberOfAudio = 0;
	m_numberOfEffects = 0;
	m_numberOfGeometry = 0;
	m_numberOfLight = 0;
	m_numberOfMaterial = 0;
	m_numberOfMesh = 0;
	m_numberOfResources = 0;
	m_numberOfPhysics = 0;
	m_numberOfTextures = 0;
}

PackageSpec::EntryHeader* PackageSpec::readHeader(const Byte* buffer, uint32& offset) {
	EntryHeader* h = (EntryHeader*)(buffer + offset);
	offset += sizeof(EntryHeader);
	return h;
}

bool PackageSpec::import(const Byte* data, size_type size)
{
	const Byte* buffer = data;

	uint32 readOffset = 0;

	uint32 trackedFiles = *(uint32*)(buffer + readOffset);
	readOffset += (trackedFiles + 1 ) * sizeof(uint32);
	
	m_numberOfResources = *(uint32*)(buffer + readOffset);
	readOffset += sizeof(uint32);

	uint32 offset = readOffset + m_numberOfResources * sizeof(EntryHeader); //The first part of the package contains a library, right after the entries, the resources starts

	uint32 res_readOffset = readOffset, res_offset = offset;

	for (uint32 res = 0; res < m_numberOfResources; ++res) {
		//read in resource headers
		EntryHeader* header = readHeader(buffer, readOffset);

		const Byte* resourceMem = buffer + offset;
		offset += header->size;

		switch (header->type) {
		case ResourceType::Animation: ++m_numberOfAnimation; break;
		case ResourceType::Audio: ++m_numberOfAudio; break;
		case ResourceType::Effect: ++m_numberOfEffects; break;
		case ResourceType::Geometry: ++m_numberOfGeometry; break;
		case ResourceType::Light: ++m_numberOfLight; break;
		case ResourceType::Material: ++m_numberOfMaterial; break;
		case ResourceType::Mesh: ++m_numberOfMesh; break;
        case ResourceType::Physics: ++m_numberOfPhysics; break;
		case ResourceType::Texture: ++m_numberOfTextures; break;
		};
	}

	if (m_numberOfAnimation > 0) 
		m_Animations = eng_new_N(CAnSPtr, m_numberOfAnimation, *m_PkgAllocator);
	if (m_numberOfAudio > 0) 
		m_Audios = eng_new_N(CAuSPtr, m_numberOfAudio, *m_PkgAllocator);
	if (m_numberOfEffects > 0)
		m_Effects = eng_new_N(CEfSPtr, m_numberOfEffects, *m_PkgAllocator);
	if (m_numberOfGeometry > 0)
		m_Geometries = eng_new_N(CGeSPtr, m_numberOfGeometry, *m_PkgAllocator);
	if (m_numberOfLight > 0) 
		m_Lights = eng_new_N(CLiSPtr, m_numberOfLight,* m_PkgAllocator);
	if (m_numberOfMaterial > 0) 
		m_Materials = eng_new_N(CMaSPtr, m_numberOfMaterial, *m_PkgAllocator);
	if (m_numberOfMesh > 0) 
		m_Meshs = eng_new_N(CMeSPtr, m_numberOfMesh, *m_PkgAllocator);
	if (m_numberOfPhysics > 0)
		m_Physics = eng_new_N(CPhSPtr, m_numberOfPhysics, *m_PkgAllocator);
	if (m_numberOfTextures > 0)
		m_Textures = eng_new_N(CTeSPtr, m_numberOfTextures, *m_PkgAllocator);

	readOffset = res_readOffset;
	offset = res_offset;
	int32 an = 0, au = 0, eff = 0, geo = 0, li = 0, ma = 0, me = 0, ph = 0, te = 0;
	for (uint32 res = 0; res < m_numberOfResources; ++res) {
		//read in resource headers
		EntryHeader* header = readHeader(buffer, readOffset);

		const Byte* resourceMem = buffer + offset;
		offset += header->size;

		switch (header->type) {
		case ResourceType::Animation: 
		{
			m_Animations[an++] = AnimationSpec::FromBuffer(resourceMem);
		}
		break;
		case ResourceType::Audio:
		{
			m_Audios[au++] = AudioSpec::FromBuffer(resourceMem);
		}
		break;
		case ResourceType::Effect:
		{
			m_Effects[eff++] = EffectSpec::FromBuffer(resourceMem);
		}
		break;
		case ResourceType::Geometry:
		{
			const GeometrySpec* geometry = GeometrySpec::FromBuffer(resourceMem);
			m_Geometries[geo++] = geometry;
		}
		break;
		case ResourceType::Light:
		{
			m_Lights[li++] = LightSpec::FromBuffer(resourceMem);
		}
		break;
		case ResourceType::Material:
		{
			m_Materials[ma++] = MaterialSpec::FromBuffer(resourceMem);
		}
		break;
		case ResourceType::Mesh:
		{
			m_Meshs[me++] = MeshSpec::FromBuffer(resourceMem);
		}
		break;
        case ResourceType::Physics:
		{
			m_Physics[ph++] = PhysicsSpec::FromBuffer(resourceMem);
		}
		case ResourceType::Texture:
		{
			m_Textures[te++] = TextureSpec::FromBuffer(resourceMem);
		}
		break;
		}
	};

	return true;
}

ENDNAMESPACE
