#pragma once

#include "Filesystem/File.h"
#include "Filesystem/OsFile.h"

BEGINNAMESPACE

class DiskFile : public File{
    friend class Filesystem;
    friend class FileDevice;
public:
    DiskFile(FileDevice* device, const ansichar* path, FileModeFlags filemode);
    virtual ~DiskFile();

    virtual size_type read(Byte* buffer, size_type length);
    virtual size_type getline(Byte* buffer, size_type length);
    virtual size_type write(const Byte* buffer, size_type length);

    virtual void seek(size_type position);
    virtual void seekToEnd();
    virtual void skip(size_type length);
    virtual size_type tell();
    virtual bool valid() const;
protected:
    OsFile m_OsFile;
};

ENDNAMESPACE
