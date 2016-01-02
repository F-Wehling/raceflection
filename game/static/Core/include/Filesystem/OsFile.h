#pragma once

#include "Filesystem/FilesystemTypes.h"

#if OS_WINDOWS 
typedef struct _iobuf FILE;
#elif OS_LINUX
typedef struct _IO_FILE FILE;
#endif

BEGINNAMESPACE

class OsFile {
    friend class Filesystem;
    friend class FileDevice;
public:
    OsFile(const ansichar* path, FileModeFlags mode);
    ~OsFile();

    size_type read(Byte* buffer, size_type length);
    size_type getline(Byte* buffer, size_type length);
    size_type write(const Byte* buffer, size_type length);
    void seek(size_type position);
    void seekToEnd();
    void skip(size_type length);
    size_type tell();

    bool valid() const;

protected:
#   if OS_WINDOWS || OS_LINUX
    FILE* m_File;
#   endif
};

ENDNAMESPACE
