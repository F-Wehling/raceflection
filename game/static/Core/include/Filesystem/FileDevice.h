#pragma once

#include "Filesystem/FilesystemTypes.h"

BEGINNAMESPACE

class FileDevice {
public:
    FileDevice(const ansichar* deviceId);
    virtual ~FileDevice();
    virtual File* open(const ansichar* path, FileModeFlags flags) = 0;
    virtual File* open(File* file,const ansichar* path, FileModeFlags flags) = 0;
    virtual void close(File* file) = 0;

    const ansichar* getId() const;
protected:
    const ansichar* m_DeviceId;
};

ENDNAMESPACE
