#pragma once

#include "Filesystem/FilesystemTypes.h"
#include "Filesystem/FileDevice.h"

BEGINNAMESPACE

class DiskFile;

class DiskFileDevice : public FileDevice {
public:
    DiskFileDevice();
    virtual ~DiskFileDevice();
    virtual File* open(const ansichar* path, FileModeFlags flags);
    virtual File* open(File* file,const ansichar* path, FileModeFlags flags);
    virtual void close(File* file);

    const ansichar* getId() const;
};

ENDNAMESPACE
