#include "Filesystem/DiskFileDevice.h"
#include "Filesystem/DiskFile.h"

#include "MemorySystem.h"
#include "Logging/Logging.h"

BEGINNAMESPACE

DiskFileDevice::DiskFileDevice() : FileDevice("disk"){

}

DiskFileDevice::~DiskFileDevice(){

}

File* DiskFileDevice::open(const ansichar *path, FileModeFlags flags){
    DiskFile* file = eng_new(DiskFile)(this, path, flags);
    if(!file || !file->valid()) return nullptr;
    return file;
}

File* DiskFileDevice::open(File *file, const ansichar *path, FileModeFlags flags){
    LOG_ERROR(General, "A disk-file can only be use at the end of a deviceList.");
    return nullptr;
}

void DiskFileDevice::close(File *file){
    if(file){
        eng_delete(file);
    }
}

ENDNAMESPACE
