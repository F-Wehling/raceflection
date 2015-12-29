#include "PackageSystem/PackageSystem.h"

#include "PackageSpec.h"

#include "Configuration/ConfigSettings.h"

#include "Multithreading/JobScheduler.h"

#include "Filesystem/Filesystem.h"
#include "Filesystem/File.h"

#include "Logging.h"

BEGINNAMESPACE

extern ConfigSettingAnsichar cfgPathPrefix;
extern ConfigSettingAnsichar cfgPackageToImport;

ConfigSettingUint32 cfgPackageStorage("PackageStorage", "The storage for a PackageImport-System", MEGABYTE(128));
ConfigSettingAnsichar cfgPackageDevice("PackageDevice", "The device-specifiation for package files", "memfile:disk");

PackageSystem::PackageSystem() :
	m_PkgAllocator("PackageAllocator"),
	m_ImportHdlAllocator("ImportHandleAllocator")
{
	m_PkgAllocator.initialize(cfgPackageStorage);
	m_ImportHdlAllocator.initialize(12 * sizeof(ImportHandle), sizeof(ImportHandle), alignof(ImportHandle), 0);
}

ImportHandle* PackageSystem::startImportPackage(const ansichar* importFile)
{
	ImportHandle* hdl = eng_new(ImportHandle, m_ImportHdlAllocator);
	strcpy(hdl->_importFile, importFile);
	hdl->_importMemory = nullptr;
	hdl->_importSize = 0;
	hdl->_refSystem = this;
	hdl->_pkg = nullptr;
	hdl->_importJob = JobScheduler::CreateJob(PackageSystem::Import, hdl);

	JobScheduler::Run(hdl->_importJob);
	return hdl;
}

bool PackageSystem::isFinished(ImportHandle* hdl)
{
	return JobScheduler::HasJobCompleted(hdl->_importJob);
}

PackageSpec* PackageSystem::interprete(ImportHandle* hdl)
{
	JobScheduler::Wait(hdl->_importJob); //wait for the job to finish

	if (!hdl || !hdl->_importMemory || !hdl->_pkg || hdl->_importSize == 0) {
		LOG_ERROR(General, "File import failed.");
		return false;
	}
	
	if (!hdl->_pkg->import(hdl->_importMemory, hdl->_importSize)) {
		return nullptr;
	}
	return hdl->_pkg;
}

Filesystem g_FilesysInterface;

void PackageSystem::Import(Job * job, const void * data)
{
	ImportHandle* importHandle = (ImportHandle*)(*((UIntOfPtrSize*)data));
	PackageSystem* _this = importHandle->_refSystem;

    File* file = g_FilesysInterface.open(importHandle->_importFile, cfgPackageDevice, FileMode::Read | FileMode::Binary);

    if(!file) return;

    file->seekToEnd();

	IFileStream ifile;

	ifile.open(importHandle->_importFile, std::ios::binary | std::ios::in | std::ios::ate);

	if (!ifile.is_open()) return;
	
    size_type size = file->tell();
    file->seek(0);
	
	importHandle->_importMemory = eng_new_N(Byte, size, _this->m_PkgAllocator);
	importHandle->_importSize = size;
    file->read(importHandle->_importMemory, size);
    g_FilesysInterface.close(file);

	importHandle->_pkg = eng_new(PackageSpec, _this->m_PkgAllocator)(&_this->m_PkgAllocator);
}

void PackageSystem::tick(float32 dt) {

}

ENDNAMESPACE

