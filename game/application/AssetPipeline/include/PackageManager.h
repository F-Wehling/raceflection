#pragma once

#include <vector>
#include "Filesystem.h"

BEGINNAMESPACE

class Package;

class PackageManager {
	using path = filesys::path;
public:
	PackageManager(const path& package_root);
	~PackageManager();

	bool loadPackages();

	const path& getPackageRoot() const;
	const path& getAnimationRoot() const;
	const path& getAudioRoot() const;
	const path& getEffectRoot() const;
	const path& getLightRoot() const;
	const path& getMaterialRoot() const;
	const path& getMeshRoot() const;
    const path& getPhysicsRoot() const;
	const path& getSceneRoot() const;
	const path& getTextureRoot() const;

	bool shutdown() const;

	bool tick();

private:
	void trackNewPackages(const path& path);
	
	void initialize_dirs();
	void initialize_dir(const path& dir, const ansichar* name, bool optional);

	void update();
	void store();
private:
	std::vector<Package*> m_Packages;
	uint32 m_RecentTimestamp;
	path m_PackageRoot;
	path m_AnimationRoot; //Root of the animation files (normally part of the scene files);
	path m_AudioRoot; //Root of the audio files
	path m_EffectRoot; //Root of the effect files
	path m_LightRoot; //root of the light definition (normally part of the scene files)
	path m_MaterialRoot; //root of the material files (normally part of the scene files)
	path m_MeshRoot; //root of the mesh files (normally part of the scene files)
    path m_PhysicsRoot; //root of the physic files
	path m_SceneRoot; //Root of the scene files (COLLADA, FBX,...)
	path m_TextureRoot; //Root of the dds-textures

	bool m_Shutdown;
};

ENDNAMESPACE
