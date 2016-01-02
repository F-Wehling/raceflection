#include "PackageSystem/PackageSystem.h"

#include "PackageSpec.h"

#include "Configuration/ConfigSettings.h"

#include "Multithreading/JobScheduler.h"

#include "Filesystem/Filesystem.h"
#include "Filesystem/File.h"

#include "Logging.h"

// include all systems to update whenever a new package was loaded
#include "Main.h"
#include "RenderSystem/RenderSystem.h"
#include "EffectSystem/EffectSystem.h"

BEGINNAMESPACE

extern ConfigSettingAnsichar cfgPathPrefix;
extern ConfigSettingAnsichar cfgPackageToImport;

ConfigSettingUint32 cfgPackageStorage("PackageStorage", "The storage for a PackageImport-System", MEGABYTE(128));
ConfigSettingAnsichar cfgPackageDevice("PackageDevice", "The device-specifiation for package files", "memory:disk");
ConfigSettingBool cfgPackageAutoreload("PackageAutoreload", "Wheter to autoreload updated packages", true);

PackageSystem::PackageSystem(Main* main) :
	m_MainRef(main),
	m_PkgAllocator("PackageAllocator"),
	m_ImportHdlAllocator("ImportHandleAllocator"),
	m_Filesystem(nullptr)
{
	//reserve memory
	m_ImportHandles.reserve(10);
	m_TrackedPackages.reserve(10); 
}

PackageSystem::~PackageSystem() {
	shutdown();
}

void PackageSystem::importPackage(const ansichar* importFile)
{
	startImportPackage(importFile, nullptr);
}

bool PackageSystem::initialize()
{
	m_PkgAllocator.initialize(cfgPackageStorage);
	m_ImportHdlAllocator.initialize(12 * sizeof(ImportHandle), sizeof(ImportHandle), alignof(ImportHandle), 0);
	m_Filesystem = eng_new(Filesystem, m_PkgAllocator);
	return true;
}

void PackageSystem::shutdown() {
	for (TrackedPackage& pkg : m_TrackedPackages) {
		eng_delete(pkg.packageInstance, m_PkgAllocator);
		eng_delete(pkg.packageMemory, m_PkgAllocator);
	}
	eng_delete(m_Filesystem, m_PkgAllocator);
}

void PackageSystem::startImportPackage(const ansichar * importFile, TrackedPackage * replace)
{
	m_ImportHandles.push_back(ImportHandle());
	ImportHandle* hdl = &m_ImportHandles.back();
	std::memset(hdl, 0, sizeof(ImportHandle));
	strcpy(hdl->_importFile, importFile);
	hdl->_importMemory = nullptr;
	hdl->_importSize = 0;
	hdl->_refSystem = this;
	hdl->_pkg = nullptr;
	hdl->_importJob = JobScheduler::CreateJob(PackageSystem::Import, hdl);
	hdl->_replacePkg = replace;

	JobScheduler::Run(hdl->_importJob); //start imorting on a free thread
}

bool PackageSystem::isFinished(ImportHandle& hdl)
{
	return JobScheduler::HasJobCompleted(hdl._importJob);
}

bool PackageSystem::interprete(ImportHandle& hdl)
{
	JobScheduler::Wait(hdl._importJob); //wait for the job to finish

	if (!hdl._importMemory || !hdl._pkg || hdl._importSize == 0) {
		LOG_ERROR(General, "File import failed.");
        return false;
	}

	uint32 timeStamp = Filesystem::LastWriteTime(hdl._importFile); //get last write time to auto-update
	
	if (!hdl._pkg->import(hdl._importMemory, hdl._importSize)) {
		return false;
	}
	TrackedPackage tracked;
	std::memset(&tracked, 0, sizeof(TrackedPackage));
	std::strcpy(tracked.packageFile, hdl._importFile);
	tracked.time_stamp = timeStamp;
	tracked.packageMemory = hdl._importMemory; //handover the memory to the tracked package
	tracked.packageInstance = hdl._pkg;

	//Create/Update all systems
	if (hdl._replacePkg != nullptr) { //replaced package
		//release old package-memory
		eng_delete(hdl._replacePkg->packageInstance, m_PkgAllocator);
		eng_delete_array(hdl._replacePkg->packageMemory, m_PkgAllocator);
		std::memcpy(hdl._replacePkg, &tracked, sizeof(TrackedPackage)); //store new handle

		m_MainRef->getRenderSystemPtr()->updateResourcesFromPackage(hdl._pkg);
		m_MainRef->getEffectSystemPtr()->updateEffectLibraryFromPackageSpec(hdl._pkg);
	}
	else { //new package
		m_TrackedPackages.push_back(tracked);
		m_MainRef->getRenderSystemPtr()->createResourcesFromPackage(hdl._pkg);
		m_MainRef->getEffectSystemPtr()->createEffectLibraryFromPackageSpec(hdl._pkg);
	}
	return true;
}

void PackageSystem::Import(Job * job, const void * data)
{
	ImportHandle* importHandle = (ImportHandle*)(*((UIntOfPtrSize*)data));
	PackageSystem* _this = importHandle->_refSystem;

    File* file =_this->m_Filesystem->open(cfgPackageDevice, importHandle->_importFile, FileMode::Read | FileMode::Binary);

    if(!file) return;

    file->seekToEnd();

    size_type size = file->tell();
    file->seek(0);
	
	importHandle->_importMemory = eng_new_N(Byte, size, _this->m_PkgAllocator);
	std::memset(importHandle->_importMemory, 0, size);
	importHandle->_importSize = size;
    file->read(importHandle->_importMemory, size);
    _this->m_Filesystem->close(file);

	importHandle->_pkg = eng_new(PackageSpec, _this->m_PkgAllocator)(&_this->m_PkgAllocator);
}

void PackageSystem::tick(float32 dt) {

	for (ImportHandles_t::iterator it = m_ImportHandles.begin(); it != m_ImportHandles.end(); ++it) {
		ImportHandle* hdl = &*it;
		if (JobScheduler::HasJobCompleted(hdl->_importJob)) { //job finished -> start interprete it
			if (!interprete(*hdl)) {
				LOG_ERROR(General, "Import package-file %s failed.", hdl->_importFile);
			}		
			hdl->_state = 1; //mark for delete
		}
	}

	uint32 deleteHdlCnt = 0;
	for (ImportHandles_t::iterator it = m_ImportHandles.begin(); it != m_ImportHandles.end(); ++it) {
		if (it->_state == 1) {
			ImportHandle tmp;
			if (&*it != &m_ImportHandles.back()) {
				std::memcpy(&tmp, &*(m_ImportHandles.rbegin() + deleteHdlCnt), sizeof(ImportHandle));
				std::memcpy(&*(m_ImportHandles.rbegin() + deleteHdlCnt), &*it, sizeof(ImportHandle));
				std::memcpy(&*it, &tmp, sizeof(ImportHandle));
			}
			deleteHdlCnt++;
		}
	}
	m_ImportHandles.resize(m_ImportHandles.size() - deleteHdlCnt);

	if (!cfgPackageAutoreload) return;

	for (TrackedPackages_t::iterator it = m_TrackedPackages.begin(); it != m_TrackedPackages.end(); ++it) {
		if (it->time_stamp < Filesystem::LastWriteTime(it->packageFile) && it->_state == 0) {
			//reimport this package
			LOG_INFO(General, "--------------------\nReload package: %s\n--------------------", it->packageFile);
			it->_state = 1; //will be reimported
			startImportPackage(it->packageFile, &*it);
		}
	}
}

ENDNAMESPACE

