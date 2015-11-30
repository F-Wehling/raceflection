# This file is a general version for library generation
# so the CMakeLists.txt only need to include this file after specifying the Target_Name (and some optional variables to define dependencies)

include(${cmake_root}/find_files.cmake)

set(Target_Name "static_${Target_Name}")

add_library(${Target_Name} STATIC ${Target_src} ${Target_h} )

#if("${Target_Name}" STREQUAL "static_Core")
#	add_definitions("-DCORE_EXPORT=1")
#endif()

set(Target_Lib ${static_libraries} ${Target_Lib})
set( static_libraries ${static_libraries} ${Target_Name} PARENT_SCOPE)
	
include(${cmake_root}/global.cmake)

message("Name: ${Target_Name}")
message("---LIBRARY-STATIC-----------")
message("---PRJECT-------------------")
message("----------------------------")
message("")
message("")