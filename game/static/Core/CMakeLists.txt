set(Target_Name Core)

#do not include hash-lib if not needed
#include( "${cmake_root}/hash.cmake")

include("../CMakeStaticLibGeneration.cmake")