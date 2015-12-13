#include <PackageManager.h>
#include <Package.h>
#include <Logging/Logging.h>

BEGINNAMESPACE

static const ansichar* PackageExtension = ".pkg";

PackageManager::PackageManager(const path& package_root) : m_Shutdown(false), m_RecentTimestamp(0){
	
	if (!is_directory(package_root)) {
		LOG_ERROR(General, "Initialize the package manager with the folder for the assets files.");
		m_Shutdown = true;
		return;
	}

	m_PackageRoot = package_root / path("packages/");
	m_AnimationRoot = package_root / path("animations/");
	m_AudioRoot = package_root / path("audio/");
	m_LightRoot = package_root / path("lights/");
	m_MaterialRoot = package_root / path("materials/");
	m_MeshRoot = package_root / path("meshes/");
	m_SceneRoot = package_root / path("scenes/");
	m_TextureRoot = package_root / path("textures/");

	initialize_dirs();
}

PackageManager::~PackageManager()
{
	for (auto pkg : m_Packages) { delete pkg; }
}

bool PackageManager::loadPackages()
{
	//load all pkg files from the base path
	for (auto& it : recursive_directory_iterator(m_PackageRoot)) {
		path entry = it.path();
		if (!is_regular_file(entry)) continue;
		if (entry.extension() != ".pkg") continue;

		Package* pkg = new Package;
		if (!pkg->load(entry, *this)) {
			delete pkg;
			continue;
		}
		m_RecentTimestamp = std::max(m_RecentTimestamp, pkg->getRecentTimeStamp());
		m_Packages.push_back(pkg);
	}
	return true;
}

const path & PackageManager::getPackageRoot() const
{
	return m_PackageRoot;
}

const path & PackageManager::getAnimationRoot() const
{
	return m_AnimationRoot;
}

const path & PackageManager::getAudioRoot() const
{
	return m_AudioRoot;
}

const path & PackageManager::getLightRoot() const
{
	return m_LightRoot;
}

const path & PackageManager::getMaterialRoot() const
{
	return m_MaterialRoot;
}

const path & PackageManager::getMeshRoot() const
{
	return m_MeshRoot;
}

const path & PackageManager::getSceneRoot() const
{
	return m_SceneRoot;
}

const path & PackageManager::getTextureRoot() const
{
	return m_TextureRoot;
}

bool PackageManager::shutdown() const
{
	return m_Shutdown;
}

bool PackageManager::tick()
{
	// update all already loaded packages
	for (auto res : m_Packages) {
		res->update(*this);
		res->store(*this);
	}
	trackNewPackages(m_AnimationRoot);
	//check if there are new packages to track
	trackNewPackages(m_AnimationRoot);
	trackNewPackages(m_AudioRoot);
	trackNewPackages(m_LightRoot);
	trackNewPackages(m_MaterialRoot);
	trackNewPackages(m_MeshRoot);
	trackNewPackages(m_SceneRoot);
	trackNewPackages(m_TextureRoot);
	return true;
}

void PackageManager::trackNewPackages(const path & dir) {
	for (auto file_entry : directory_iterator(dir)) {
		path folder = file_entry.path();
		if (!is_directory(folder)) continue;

		String packageName = folder.stem().string();

		if (std::find_if(m_Packages.begin(), m_Packages.end(), [&](Package* pkg)->bool { return packageName == pkg->getPackageName(); }) != m_Packages.end()) continue; //This package is already tracked
		
		Package* pkg = new Package(packageName, *this);
		pkg->update(*this); //load all files into this package
		m_Packages.push_back(pkg);
	}
}

void PackageManager::initialize_dirs()
{
	initialize_dir(m_AnimationRoot, "animation", true);
	initialize_dir(m_AudioRoot, "audio", false);
	initialize_dir(m_LightRoot, "light", true);
	initialize_dir(m_MaterialRoot, "material", true);
	initialize_dir(m_MeshRoot, "mesh", true);
	initialize_dir(m_SceneRoot, "scene", false);
	initialize_dir(m_TextureRoot, "texture", false);
	if (!is_directory(m_PackageRoot)) {
		create_directory(m_PackageRoot);
	}
	path info_file = m_PackageRoot / "Readme.txt";

	if (!exists(info_file)) {
		OFileStream infofile(info_file.c_str(), std::ios::out);
		if (infofile.is_open()) {
			infofile << "This folder  contains all packaged data." << std::endl;
			infofile.close();
		}
	}
}

void PackageManager::initialize_dir(const path & dir, const ansichar * name, bool optional)
{
	if (!is_directory(dir)) {
		create_directory(dir);
	}
	
	path info_file = dir / "Readme.txt";

	if (!exists(info_file)) {
		OFileStream infofile(info_file.c_str(), std::ios::out);
		if (infofile.is_open()) {
			infofile << "This folder must contains all " << name << " data that should be packaged." << std::endl;
			if (optional) {
				infofile << "By default all " << name << " data is already contained in the scene files." << std::endl;
			}
			infofile << std::endl << "Create subfolders to indicate a package (foldername = packagename)" << std::endl;
			infofile.close();
		}
	}
}

void PackageManager::update()
{
	for (Package* pkg : m_Packages) {
		pkg->update(*this);
	}
}

void PackageManager::store()
{
	for (Package* pkg : m_Packages) {
		pkg->store(*this);
	}
}


ENDNAMESPACE