#include "Filesystem/MemoryFile.h"
#include "Filesystem/FileDevice.h"

#include "MemorySystem.h"

#include "Logging/Logging.h"
#include <sstream>

BEGINNAMESPACE

MemoryFile::MemoryFile(FileDevice* device, File* file) :
    File(device),
    m_ContainedFile(file),
    m_MemStream()
{
    if(!m_ContainedFile || !m_ContainedFile->valid()) {
        return;
    }
    m_ContainedFile->seekToEnd();
    m_StreamEnd = m_ContainedFile->tell();
    m_ContainedFile->seek(0);
    m_MemFile = eng_new_N(Byte, m_StreamEnd);
    m_ContainedFile->read(m_MemFile, m_StreamEnd);

    new (&m_MemStream)ByteStream(m_MemFile, m_StreamEnd);
}

MemoryFile::~MemoryFile(){
    eng_delete_array(m_MemFile);
}


size_type MemoryFile::read(Byte* buffer, size_type length){
    size_type t = tell();
    return m_MemStream.read((ansichar*)buffer, length).tellg() - t;
}

size_type MemoryFile::getline(Byte* buffer, size_type length){
    size_type t = tell();
    return m_MemStream.getline((ansichar*)buffer, length).tellg() - t;
}

size_type MemoryFile::write(const Byte* buffer, size_type length){
    m_ContainedFile->write(buffer, length);
}

void MemoryFile::seek(size_type position){
    m_MemStream.seekg(position, std::ios::beg);
}

void MemoryFile::seekToEnd(){
    m_MemStream.seekg(0, std::ios::end);
}

void MemoryFile::skip(size_type length){
    m_MemStream.seekg(length, std::ios::cur);
}

size_type MemoryFile::tell(){
    return m_MemStream.tellg();
}

bool MemoryFile::valid() const {
    return m_MemFile != nullptr;
}

ENDNAMESPACE
