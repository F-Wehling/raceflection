#include "Filesystem/MemoryFileDevice.h"
#include "Filesystem/MemoryFile.h"

#include "MemorySystem.h"
#include "Logging/Logging.h"

BEGINNAMESPACE

MemoryFileDevice::MemoryFileDevice() : FileDevice("memory") {
}

MemoryFileDevice::~MemoryFileDevice(){

}

File* MemoryFileDevice::open(const ansichar *path, FileModeFlags flags){
    LOG_ERROR(General, "A memory-file can not be use at the end of a deviceList.");
    return nullptr;
}

File* MemoryFileDevice::open(File *file, const ansichar *path, FileModeFlags flags){
    MemoryFile* f = eng_new(MemoryFile)(this, file);
    return f;
}

void MemoryFileDevice::close(File *file){
    if(file){
        MemoryFile* memFile = (MemoryFile*)file;
        File* containedFile = memFile->m_ContainedFile;
        if(containedFile && containedFile->getDevice())
            containedFile->getDevice()->close(containedFile);
        eng_delete(memFile);
    }
}

ENDNAMESPACE
