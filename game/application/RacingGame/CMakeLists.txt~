set( Target_Name "RacingGame")

include("${cmake_root}/glfw.cmake")
include("${cmake_root}/OIS.cmake")
include("${cmake_root}/acgl.cmake")
include("${cmake_root}/OpenGL.cmake")
include("${cmake_root}/bullet3.cmake")

if(UNIX)
    set(Target_Lib ${Target_Lib} -pthread)
endif()

include("../CMakeApplicationGeneration.cmake")
