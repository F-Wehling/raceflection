#pragma once

#include "Utilities/Flags.h"

BEGINNAMESPACE

DECLARE_FLAGS(FileMode, Read, Write, WriteTrunc, Binary);

class File;
class Filesystem;
class FileDevice;

ENDNAMESPACE
