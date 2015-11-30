###---
### include the correct boost lib and include files
###
message("\tExternal: OIS")
include(FindPkgMacros)

set(OIS_INCLUDE_DIR ${extern_root}/include/OIS)
set(OIS_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc "${Target_Inc} ${OGRE_INCLUDE_DIR}")

set(OIS_LIBRARY_NAMES OIS)
get_debug_names(OIS_LIBRARY_NAMES)

find_library(OIS_LIBRARY_REL NAMES ${OIS_LIBRARY_NAMES} HINTS ${OIS_LIBRARY_DIR} PATH_SUFFIXES "" release Release)
find_library(OIS_LIBRARY_DBG NAMES ${OIS_LIBRARY_NAMES_DBG} HINTS ${OIS_LIBRARY_DIR} PATH_SUFFIXES "" debug Debug)

make_library_set(OIS_LIBRARY)

findpkg_finish(OIS)

set(Target_Lib ${Target_Lib} ${OIS_LIBRARY})

#set(Target_Release_Lib ${Target_Release_Lib} ${OGRE_Lib_Release})
#set(Target_Debug_Lib ${Target_Debug_Lib} ${OGRE_Lib_Debug})