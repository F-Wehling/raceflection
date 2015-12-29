#pragma once

#include "Filesystem/FilesystemTypes.h"

BEGINNAMESPACE

class File {
    friend class Filesystem;
    friend class FileDevice;
public:
    File(FileDevice* device);
    virtual ~File();

    virtual size_type read(Byte* buffer, size_type length) = 0;
    virtual size_type getline(Byte* buffer, size_type length) = 0;
    virtual size_type write(const Byte* buffer, size_type length) = 0;

    virtual void seek(size_type position) = 0;
    virtual void seekToEnd() = 0;
    virtual void skip(size_type length) = 0;
    virtual size_type tell() = 0;
    virtual bool valid() const = 0;

    FileDevice* getDevice() { return m_Device; }

protected:
    FileDevice* m_Device;
};

ENDNAMESPACE
