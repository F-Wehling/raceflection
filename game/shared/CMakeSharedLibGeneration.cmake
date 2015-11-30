# This file is a general version for shared-lib generation
# so the CMakeLists.txt only need to include this file after specifying the Target_Name  (and some optional variables to define dependencies)

set(Target_Name "shared_${Target_Name}") #prepend shared_ to library name

include(${cmake_root}/find_files.cmake)

set( shared_libraries ${shared_libraries} ${Target_Name} PARENT_SCOPE)

set(Target_Lib ${Target_Lib} ${static_libraries}) #link all shared-libraries against all static

# generate a new target
add_library(${Target_Name} SHARED ${Target_src} ${Target_h} )

add_definitions("-DDLL_EXPORT=1")

include(${cmake_root}/global.cmake)

message("Name: ${Target_Name}")
message("---LIBRARY-SHARED-----------")
message("---STATIC-------------------")
message("----------------------------")
message("")
message("")