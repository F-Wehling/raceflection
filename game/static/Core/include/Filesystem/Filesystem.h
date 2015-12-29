#pragma once

#include "Filesystem/FilesystemTypes.h"

#include "Container/Array.h"
#include "MemorySystem.h"

BEGINNAMESPACE

class Filesystem {
public:
    Filesystem();
    ~Filesystem();

    template<typename _FileDevice>
    void mount() {
        _FileDevice* dev = eng_new(_FileDevice);
        m_deviceList.push_back(dev);
    }

    void mount(FileDevice* dev);

    void unmount(FileDevice* device);

    File* open(const ansichar* deviceList, const ansichar* path, FileModeFlags mode);
    void close(File* file);

private:
    File* _rec_open(const ansichar* deviceList, const ansichar* path, FileModeFlags mode);
    FileDevice* _find_Device(const ansichar* id, size_type length);

    typedef DynArray<FileDevice*> DevList_t;
    DevList_t m_deviceList;
};


ENDNAMESPACE
