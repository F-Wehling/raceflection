#include <Package.h>

#include "Logging/Logging.h"

#include <ImportAnimation.h>
#include <ImportAudio.h>
#include <ImportEffect.h>
#include <ImportLight.h>
#include <ImportMaterial.h>
#include <ImportMesh.h>
#include <ImportPhysics.h>
#include <ImportScene.h>
#include <ImportTexture.h>

#include <Utilities/Hash.h>

BEGINNAMESPACE

Package::Package(const PackageManager& mgr) : m_Dirty(false), m_RecentTimestamp(0), m_ResourceCount(0), m_RefMgr(&mgr)
{}

Package::Package(const String & pkgName, const PackageManager& mgr) : m_Dirty(false), m_packageName(pkgName), m_RecentTimestamp(0), m_ResourceCount(0), m_RefMgr(&mgr)
{	
	m_AnimationFolder = filesys::concat(mgr.getAnimationRoot(), m_packageName);
	m_AudioFolder = filesys::concat(mgr.getAudioRoot(), m_packageName);
	m_EffectFolder = filesys::concat(mgr.getEffectRoot(), m_packageName);
	m_LightFolder = filesys::concat(mgr.getLightRoot(), m_packageName);
	m_MaterialFolder = filesys::concat(mgr.getMaterialRoot(), m_packageName);
	m_MeshFolder = filesys::concat(mgr.getMeshRoot(), m_packageName);
    m_PhysicsFolder = filesys::concat(mgr.getPhysicsRoot(), m_packageName);
	m_SceneFolder = filesys::concat(mgr.getSceneRoot(), m_packageName);
	m_TextureFolder = filesys::concat(mgr.getTextureRoot(), m_packageName);
	m_PackagePath = filesys::concat(mgr.getPackageRoot(), (m_packageName + ".pkg"));
}

uint32 Package::update_scene()
{
	uint32 max_currentTimeStamp = m_RecentTimestamp;

	if (!filesys::is_directory(m_SceneFolder)) return max_currentTimeStamp; //The Package has no folder in the scene-folder

	for (filesys::DirectoryIterator file_entry(m_SceneFolder); file_entry != filesys::DirectoryIterator(); ++file_entry) {
		bool update = false;
		path file = *file_entry;
		if (!filesys::is_regular_file(file)) continue;

		uint32 currentTimeStamp = filesys::last_write_time(file);

		if (alreadyTracked(file)) {
			//This file was processed before: check wheter it has to be updated
			if (currentTimeStamp <= m_RecentTimestamp) continue; //this resource is up to date
		
			update = true;
		}

		//This file is either out of date or wasn't tracked before
	   //load from file
		if (!Importer::sceneLoad(file, *this, currentTimeStamp)) {
			LOG_ERROR(General, "The scene (%s) load failed.", file.c_str());
			continue;
		}

		setFileTracked(file);

		m_Dirty = true;

		max_currentTimeStamp = std::max(currentTimeStamp, max_currentTimeStamp);
	}
	return max_currentTimeStamp;
}

bool Package::alreadyTracked(const path & file)
{
	String filename = m_packageName + "/" + filesys::stem(file);
	uint32 hash = crc32((Byte*)filename.c_str(), filename.length());
	return std::find(m_TrackedFiles.begin(), m_TrackedFiles.end(), hash) != m_TrackedFiles.end();
}

void Package::setFileTracked(const path & file)
{
	String filename = m_packageName + "/" + filesys::stem(file);
	uint32 hash = crc32((Byte*)filename.c_str(), filename.length());
	m_TrackedFiles.push_back(hash);
}

void Package::attachTracked(uint32 hash)
{
	m_TrackedFiles.push_back(hash);
}

void Package::readHeader(Byte* data, Package::EntryHeader& header, size_type& off) {
	std::memcpy(&header, data + off, sizeof(EntryHeader));
	off += sizeof(EntryHeader);
}

//
/// Load a package from file and inject all objects right into the memory
bool Package::load(const path & filename, const PackageManager& mgr)
{
	m_PackagePath = filename;
	m_packageName = filesys::stem(m_PackagePath);
	m_AnimationFolder = filesys::concat(mgr.getAnimationRoot(), m_packageName);
	m_AudioFolder = filesys::concat(mgr.getAudioRoot(), m_packageName);
	m_EffectFolder = filesys::concat(mgr.getEffectRoot(), m_packageName);
	m_LightFolder = filesys::concat(mgr.getLightRoot(), m_packageName);
	m_MaterialFolder = filesys::concat(mgr.getMaterialRoot(), m_packageName);
	m_MeshFolder = filesys::concat(mgr.getMeshRoot(), m_packageName);
    m_PhysicsFolder = filesys::concat(mgr.getPhysicsRoot(), m_packageName);
	m_SceneFolder = filesys::concat(mgr.getSceneRoot(), m_packageName);
	m_TextureFolder = filesys::concat(mgr.getTextureRoot(), m_packageName);

	//
	/// buffer whole file
	size_type size = filesys::file_size(filename);

	FILE* ifile = fopen(filename.c_str(), "rb");

	if (!ifile) 
		return false;
	Byte* buffer = new Byte[size];
	std::memset(buffer, 0, size);
	fread((ansichar*)buffer, size, 1, ifile );
	fclose(ifile);

	size_type readOffset = 0;

	uint32 trackedFiles = *(uint32*)(buffer + readOffset);
	readOffset += sizeof(uint32);
	for (uint32 i = 0; i < trackedFiles; ++i)
	{
	attachTracked(*(uint32*)(buffer + readOffset));
		readOffset += sizeof(uint32);
	}
	m_ResourceCount = *(uint32*)(buffer + readOffset);
	readOffset += sizeof(uint32);

	uint32 offset = readOffset + m_ResourceCount * sizeof(EntryHeader); //The first part of the package contains a library, right after the entries, the resources starts
	for (uint32 res = 0; res < m_ResourceCount; ++res) {
		//read in resource headers
		EntryHeader header = { 0,0,0,0 };
		readHeader(buffer, header, readOffset);

		Byte* resourceMem = buffer + offset;
		offset += header.size;

		switch (header.type) {
		case ResourceType::Animation:
		{
			const AnimationSpec* animation = AnimationSpec::FromBuffer(resourceMem);
			Storage<AnimationSpec> entry = { path(), animation, header };
			m_Animations.push_back(entry);
		}
		break;
		case ResourceType::Audio:
		{
			const AudioSpec* audio = AudioSpec::FromBuffer(resourceMem);
			Storage<AudioSpec> entry = { path(), audio, header };
			m_Audio.push_back(entry);
		}
		break;
		case ResourceType::Effect:
		{
			const EffectSpec* effect = EffectSpec::FromBuffer(resourceMem);
			Storage<EffectSpec> entry = { path(), effect, header };
			m_Effects.push_back(entry);
		}
        break;
		case ResourceType::Geometry:
		{
			const GeometrySpec* geometry = GeometrySpec::FromBuffer(resourceMem);
			Storage<GeometrySpec> entry = { path(), geometry, header };
			m_Geometries.push_back(entry);
		}
		break;
		case ResourceType::Light:
		{
			const LightSpec* light = LightSpec::FromBuffer(resourceMem);
			Storage<LightSpec> entry = { path(), light, header };
			m_Lights.push_back(entry);
		}
		break;
		case ResourceType::Material:
		{
			const MaterialSpec* material = MaterialSpec::FromBuffer(resourceMem);
			Storage<MaterialSpec> entry = { path(), material, header };
			m_Materials.push_back(entry);
		}
		break;
		case ResourceType::Mesh:
		{
			const MeshSpec* mesh = MeshSpec::FromBuffer(resourceMem);
			Storage<MeshSpec> entry = { path(), mesh, header };
			m_Meshes.push_back(entry);
		}
		break;
        case ResourceType::Physics:
		{
			const PhysicsSpec* phy = PhysicsSpec::FromBuffer(resourceMem);
			Storage<PhysicsSpec> entry = { path(), phy, header };
			m_Physics.push_back(entry);
		}
        break;
		case ResourceType::Texture:
		{
			const TextureSpec* texture = TextureSpec::FromBuffer(resourceMem);
			Storage<TextureSpec> entry = { path(), texture, header };
			m_Textures.push_back(entry);
		}
		break;
		}
	};

	m_Dirty = false; //loaded from .pkg file -> not dirty
	
	m_RecentTimestamp = filesys::last_write_time(filename);
	
	return true;
}

void Package::storeHeader(FILE* file, Package::EntryHeader& header) {
	fwrite(&header, sizeof(EntryHeader), 1, file);
}

bool Package::store(const PackageManager& mgr)
{
	if (!m_Dirty) return true;

	uint32 bytesWritten = 0;

	FILE* file = fopen(m_PackagePath.c_str(), "wb+");
		
	if (!file) return false;
	
	uint32 trackedFiles = uint32(m_TrackedFiles.size());
	fwrite(&trackedFiles, sizeof(uint32), 1, file);
	fwrite(m_TrackedFiles.data(), sizeof(uint32), trackedFiles, file);
	
	fwrite(&m_ResourceCount, sizeof(uint32), 1, file);
	
	bytesWritten += sizeof(uint32) * (trackedFiles + 2);
	
	//add a library entry for each resource
	for (auto animation : m_Animations) {
		storeHeader(file, animation.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto audio : m_Audio) {
		storeHeader(file, audio.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto effect : m_Effects) {
		storeHeader(file, effect.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto geo : m_Geometries) {
		storeHeader(file, geo.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto light : m_Lights) {
		storeHeader(file, light.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto material : m_Materials) {
		storeHeader(file, material.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto mesh : m_Meshes) {
		storeHeader(file, mesh.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto phy : m_Physics) {
		storeHeader(file, phy.header);
		bytesWritten += sizeof(EntryHeader);
	}
	for (auto texture : m_Textures) {
		storeHeader(file, texture.header);
		bytesWritten += sizeof(EntryHeader);
	}

	std::vector<Byte> buffer;

	//store the actual data right after the library
	for (auto anim : m_Animations) {
		uint32 animationSize = AnimationSpec::MemSize(anim.resource);
		buffer.resize(animationSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		AnimationSpec::ToBuffer(anim.resource, buffer.data());
		fwrite(buffer.data(), animationSize, 1, file);
		bytesWritten += animationSize;
	}
	for (auto aud : m_Audio) {
		uint32 audioSize = AudioSpec::MemSize(aud.resource);
		buffer.resize(audioSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		AudioSpec::ToBuffer(aud.resource, buffer.data());
		fwrite(buffer.data(), audioSize, 1, file);
		bytesWritten += audioSize;
	}
	for (auto eff : m_Effects) {
		uint32 effSize = EffectSpec::MemSize(eff.resource);
		buffer.resize(effSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		EffectSpec::ToBuffer(eff.resource, buffer.data());
		fwrite(buffer.data(), effSize, 1, file);
		bytesWritten += effSize;
	}
	for (auto geo : m_Geometries) {
		uint32 geoSize = GeometrySpec::MemSize(geo.resource);
		buffer.resize(geoSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		GeometrySpec::ToBuffer(geo.resource, buffer.data());
		if (1 != fwrite(buffer.data(), geoSize, 1, file)) {
			LOG_ERROR(General, "Fwrite returns an error.");
		}
		bytesWritten += geoSize;
	}
	for (auto li : m_Lights) {
		uint32 lightSize = LightSpec::MemSize(li.resource);
		buffer.resize(lightSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		LightSpec::ToBuffer(li.resource, buffer.data());
		fwrite(buffer.data(), lightSize, 1, file);
		bytesWritten += lightSize;
	}
	for (auto mat : m_Materials) {
		uint32 materialSize = MaterialSpec::MemSize(mat.resource);
		buffer.resize(materialSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		MaterialSpec::ToBuffer(mat.resource, buffer.data());
		fwrite(buffer.data(), materialSize, 1, file);
		bytesWritten += materialSize;
	}
	for (auto m : m_Meshes) {
		uint32 meshSize = MeshSpec::MemSize(m.resource);
		buffer.resize(meshSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		MeshSpec::ToBuffer(m.resource, buffer.data());
		fwrite(buffer.data(), meshSize, 1, file);
		bytesWritten += meshSize;
	}
	for (auto phy : m_Physics) {
		uint32 phySize = PhysicsSpec::MemSize(phy.resource);
		buffer.resize(phySize);
		std::fill(buffer.begin(), buffer.end(), 0);
		PhysicsSpec::ToBuffer(phy.resource, buffer.data());
		fwrite(buffer.data(), phySize, 1, file);
		bytesWritten += phySize;
	}
	for (auto tex : m_Textures) {
		uint32 textureSize = TextureSpec::MemSize(tex.resource);
		buffer.resize(textureSize);
		std::fill(buffer.begin(), buffer.end(), 0);
		TextureSpec::ToBuffer(tex.resource, buffer.data());
		fwrite(buffer.data(), textureSize, 1, file);
		bytesWritten += textureSize;
	}
	
	LOG_INFO(General, "Totaly: %d Bytes written.", bytesWritten);

	fclose(file);
	m_Dirty = false;
	
	return true;
}

void Package::update(const PackageManager & mgr)
{
	uint32 timeStamp = m_RecentTimestamp;

	timeStamp = std::max(timeStamp, updater(m_AnimationFolder, m_Animations, &Importer::animationAllFromFile, &AnimationSpec::MemSize) );
	timeStamp = std::max(timeStamp, updater(m_AudioFolder, m_Audio, &Importer::audioAllFromFile, &AudioSpec::MemSize) );
	timeStamp = std::max(timeStamp, updater(m_EffectFolder, m_Effects, &Importer::effectAllFromFile, &EffectSpec::MemSize) );
	timeStamp = std::max(timeStamp, updater(m_RefMgr->getEffectRoot(), m_Effects, &Importer::effectAllFromFile, &EffectSpec::MemSize));
	timeStamp = std::max(timeStamp, updater(m_LightFolder, m_Lights, &Importer::lightsAllFromFile, &LightSpec::MemSize) );
	timeStamp = std::max(timeStamp, updater(m_MaterialFolder, m_Materials, &Importer::materialAllFromFile, &MaterialSpec::MemSize) );
	timeStamp = std::max(timeStamp, updater(m_MeshFolder, m_Meshes, &Importer::meshAllFromFile, &MeshSpec::MemSize) );
    timeStamp = std::max(timeStamp, updater(m_PhysicsFolder, m_Physics, &Importer::physicsFromFile, &PhysicsSpec::MemSize));
	timeStamp = std::max(timeStamp, updater(m_TextureFolder, m_Textures, &Importer::textureAllFromFile, &TextureSpec::MemSize));
	timeStamp = std::max(timeStamp, update_scene());
	m_RecentTimestamp = timeStamp;
}

void Package::addAnimationFromScene(AnimationSpec * animation, const path & sceneFile, uint32 animationSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Animation, animationSize, hash, timeStamp };

	Storage<AnimationSpec> entry = { sceneFile, animation, header };
	m_Animations.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addAudioFromScene(AudioSpec * audio, const path & sceneFile, uint32 audioSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Audio, audioSize, hash, timeStamp };

	Storage<AudioSpec> entry = { sceneFile, audio, header };
	m_Audio.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addGeometryFromScene(GeometrySpec * geometry, const path & sceneFile, uint32 geometrySize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Geometry, geometrySize, hash, timeStamp };

	Storage<GeometrySpec> entry = { sceneFile, geometry, header };
	m_Geometries.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addLightFromScene(LightSpec * light, const path & sceneFile, uint32 lightSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Light, lightSize, hash, timeStamp };

	Storage<LightSpec> entry = { sceneFile, light, header };
	m_Lights.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addMaterialFromScene(MaterialSpec * material, const path & sceneFile, uint32 materialSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Material, materialSize, hash, timeStamp };

	Storage<MaterialSpec> entry = { sceneFile, material, header };
	m_Materials.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addMeshFromScene(MeshSpec * mesh, const path& sceneFile, uint32 meshSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Mesh, meshSize, hash, timeStamp };

	Storage<MeshSpec> entry = { sceneFile, mesh, header };
	m_Meshes.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

void Package::addTextureFromScene(TextureSpec * texture, const path & sceneFile, uint32 textureSize, uint32 hash, uint32 timeStamp)
{
	m_RecentTimestamp = std::max(m_RecentTimestamp, timeStamp);
	String stem = filesys::stem(sceneFile);
	EntryHeader header = { ResourceType::Texture, textureSize, hash, timeStamp };

	Storage<TextureSpec> entry = { sceneFile, texture, header };
	m_Textures.push_back(entry);
	++m_ResourceCount;
	m_Dirty = true;
}

uint32 Package::getRecentTimeStamp()
{
	return uint32(m_RecentTimestamp);
}


ENDNAMESPACE
