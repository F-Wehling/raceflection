###---
### include the correct boost lib and include files
###
message("\tExternal: ZLib")
include(FindPkgMacros)

set(ZLIB_INCLUDE_DIR ${extern_root}/include/)
set(ZLIB_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc "${Target_Inc} ${ZLIB_INCLUDE_DIR}")

set(ZLIB_LIBRARY_NAMES zlib)
get_debug_names(ZLIB_LIBRARY_NAMES)

find_library(ZLIB_LIBRARY_REL NAMES ${ZLIB_LIBRARY_NAMES} HINTS ${ZLIB_LIBRARY_DIR} PATH_SUFFIXES "" release)
find_library(ZLIB_LIBRARY_DBG NAMES ${ZLIB_LIBRARY_NAMES_DBG} HINTS ${ZLIB_LIBRARY_DIR} PATH_SUFFIXES "" debug)

make_library_set(ZLIB_LIBRARY)

findpkg_finish(ZLIB)

set(Target_Lib ${Target_Lib} ${ZLIB_LIBRARIES})

#set(Target_Release_Lib ${Target_Release_Lib} ${OGRE_Lib_Release})
#set(Target_Debug_Lib ${Target_Debug_Lib} ${OGRE_Lib_Debug})