#include "Filesystem/File.h"
#include "Filesystem/FileDevice.h"

#include "Configuration/ConfigSettings.h"

#include <algorithm>
#include <cstring>

BEGINNAMESPACE

File::File(FileDevice* device) : m_Device(device) {}

File::~File(){}

ENDNAMESPACE
