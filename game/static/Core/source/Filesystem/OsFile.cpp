#include "Filesystem/OsFile.h"

#include <stdio.h>
#include "Logging/Logging.h"

BEGINNAMESPACE

OsFile::OsFile(const ansichar* path, FileModeFlags mode) {
#if OS_WINDOWS || OS_LINUX
    ansichar m[4] = {0,0,0,0};
    uint32 pos = 0;
    if(mode & FileMode::Read){
        m[pos++] = 'r';
    }
    if(mode & FileMode::Write){
        m[pos++] = 'a';
    }
    if(mode & FileMode::WriteTrunc) {
        m[pos++] = 'w';
    }
    if(mode & FileMode::Binary){
        m[pos++] = 'b';
    }

    m_File = fopen(path, (const ansichar*) m);
#else
#endif
}

OsFile::~OsFile(){
#if OS_WINDOWS || OS_LIUX
    if(m_File != nullptr) {
        fclose(m_File);
    }
#endif
}

size_type OsFile::read(Byte* buffer, size_type length){
#if OS_WINDOWS || OS_LINUX
    if(fread(buffer, length, 1, m_File) == 1) {
        return length;
    }
    return 0;
#else
#endif
}

size_type OsFile::getline(Byte* buffer, size_type length){
#if OS_WINDOWS || OS_LINUX
    if(fgets((ansichar*)buffer, length, m_File) == (ansichar*)buffer) {
        return length;
    }
    return 0;
#else
#endif
}

size_type OsFile::write(const Byte* buffer, size_type length){
#if OS_WINDOWS || OS_LINUX
    if(fwrite(buffer, length, 1, m_File) == 0){
        return length;
    }
    return 0;
#else
#endif
}

void OsFile::seek(size_type position){
#if OS_WINDOWS || OS_LINUX
    fseek(m_File, position, SEEK_SET);
#else
#endif
}

void OsFile::seekToEnd(){
#if OS_WINDOWS || OS_LINUX
    if(fseek(m_File, 0, SEEK_END) != 0)
        LOG_ERROR(General, "seekToEnd() failed");
#else
#endif
}

void OsFile::skip(size_type length){
#if OS_WINDOWS || OS_LINUX
    fseek(m_File, length, SEEK_CUR);
#else
#endif
}

size_type OsFile::tell(){
#if OS_WINDOWS || OS_LINUX
    return ftell(m_File);
#else
#endif
}

bool OsFile::valid() const{
#if OS_WINDOWS || OS_LINUX
    return m_File != nullptr;
#else
#endif
}


ENDNAMESPACE
