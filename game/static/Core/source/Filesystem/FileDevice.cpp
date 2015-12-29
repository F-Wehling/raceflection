#include "Filesystem/FileDevice.h"

#include <algorithm>
#include <cstring>

BEGINNAMESPACE

FileDevice::FileDevice(const ansichar* deviceId) : m_DeviceId(deviceId){

}

FileDevice::~FileDevice(){

}

const ansichar* FileDevice::getId() const {
    return m_DeviceId;
}

ENDNAMESPACE
