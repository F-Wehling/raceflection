#include "Filesystem/DiskFile.h"
#include "Filesystem/FileDevice.h"

BEGINNAMESPACE

DiskFile::DiskFile(FileDevice* device, const ansichar* path, FileModeFlags filemode) :
    File(device),
    m_OsFile(path, filemode)
{}

DiskFile::~DiskFile(){}


size_type DiskFile::read(Byte* buffer, size_type length){
    return m_OsFile.read(buffer,length);
}

size_type DiskFile::getline(Byte* buffer, size_type length){
    return m_OsFile.getline(buffer,length);
}

size_type DiskFile::write(const Byte* buffer, size_type length){
    return m_OsFile.write(buffer, length);
}

void DiskFile::seek(size_type position){
    return m_OsFile.seek(position);
}

void DiskFile::seekToEnd(){
    m_OsFile.seekToEnd();
}

void DiskFile::skip(size_type length){
    m_OsFile.skip(length);
}

size_type DiskFile::tell(){
    return m_OsFile.tell();
}
bool DiskFile::valid() const {
    return m_OsFile.valid();
}

ENDNAMESPACE
