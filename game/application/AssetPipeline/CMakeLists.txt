set( Target_Name "AssetPipeline")

include(${cmake_root}/dirent.cmake)
include(${cmake_root}/assimp.cmake)
include(${cmake_root}/freeImage.cmake)

if(UNIX)
    set(Target_Lib ${Target_Lib} -lstdc++fs)
endif()

include("../CMakeApplicationGeneration.cmake")
