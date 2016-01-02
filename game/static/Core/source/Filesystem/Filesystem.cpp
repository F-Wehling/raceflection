#include "Filesystem/Filesystem.h"
#include "Filesystem/FileDevice.h"
#include "Filesystem/File.h"

#include "Configuration/ConfigSettings.h"

//
#include "Filesystem/DiskFileDevice.h"
#include "Filesystem/MemoryFileDevice.h"
//

#include <algorithm>
#include <cstring>

#include <sys/stat.h>

BEGINNAMESPACE

ConfigSettingUint32 cfgInitialNumberOfDevices("initialNumberOfDevices", "Initial number of devices per filesystem.", 5);

Filesystem::Filesystem() {
    m_deviceList.reserve(cfgInitialNumberOfDevices);

    mount<DiskFileDevice>(); //per default mount a diskFileDevice
    mount<MemoryFileDevice>(); //per default ... memoryFileDevice
}

Filesystem::~Filesystem(){
	for (FileDevice* dev : m_deviceList) {
		eng_delete(dev);
	}
    m_deviceList.clear();
}

void Filesystem::mount(FileDevice *dev){
    if(dev != nullptr){
        m_deviceList.push_back(dev);
    }
}

void Filesystem::unmount(FileDevice* device){
    DevList_t::iterator it = std::find(m_deviceList.begin(), m_deviceList.end(), device);
    FileDevice* back = m_deviceList.back();
    if(back != *it) {
        FileDevice* del = *it;
        *it = back;
        m_deviceList.back() = del;
    }
    eng_delete(m_deviceList.back());
    m_deviceList.resize( m_deviceList.size() - 1);
}

File* Filesystem::open(const ansichar *deviceList, const ansichar *path, FileModeFlags mode){
    const ansichar* deviceName = strchr(deviceList, ':');
    if(deviceName != nullptr){
        File* f = open(deviceName + 1, path, mode); //pass through till root
        if(f == nullptr) return nullptr;
        FileDevice* dev = _find_Device(deviceList, deviceName - deviceList);
        if(dev == nullptr) return nullptr;
        return dev->open(f, path, mode);
    }

    FileDevice* dev = _find_Device(deviceList, deviceName - deviceList);
    if(!dev) return nullptr;
    return dev->open(path, mode);
}

void Filesystem::close(File *file){
    if(file){
        FileDevice* dev = file->getDevice();
        dev->close(file);
    }
}

uint32 Filesystem::LastWriteTime(const ansichar * path)
{
	struct stat file_stat;
	if (stat(path, &file_stat) != 0) return 0;
	return file_stat.st_mtime;
}

FileDevice* Filesystem::_find_Device(const ansichar* id, size_type length){
    for(FileDevice* dev : m_deviceList) {
        if(strncmp(id, dev->getId(), length) == 0){
            return dev;
        }
    }
    return nullptr;
}

ENDNAMESPACE
