#pragma once

#include "Filesystem/File.h"
#include "Filesystem/OsFile.h"

#include <Utilities/ByteStream.h>

BEGINNAMESPACE

class MemoryFile : public File{
    friend class Filesystem;
    friend class FileDevice;
    friend class MemoryFileDevice;
public:
    MemoryFile(FileDevice* device, File* file);
    virtual ~MemoryFile();

    virtual size_type read(Byte* buffer, size_type length);
    virtual size_type getline(Byte* buffer, size_type length);
    virtual size_type write(const Byte* buffer, size_type length);

    virtual void seek(size_type position);
    virtual void seekToEnd();
    virtual void skip(size_type length);
    virtual size_type tell();
    virtual bool valid() const;
protected:
    File* m_ContainedFile;
    Byte* m_MemFile;
    ByteStream m_MemStream;
    size_type m_StreamPosition;
    size_type m_StreamEnd;
};

ENDNAMESPACE
