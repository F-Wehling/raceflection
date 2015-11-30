###---
### include the correct assimp lib and include files
###
message("\tExternal: Assimp")
include(FindPkgMacros)

set(ASSIMP_INCLUDE_DIR ${extern_root}/include)
set(ASSIMP_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc "${Target_Inc} ${ASSIMP_INCLUDE_DIR}")

set(ASSIMP_LIBRARY_NAMES assimp)
get_debug_names(ASSIMP_LIBRARY_NAMES)

find_library(ASSIMP_LIBRARY_REL NAMES ${ASSIMP_LIBRARY_NAMES} HINTS ${ASSIMP_LIBRARY_DIR} PATH_SUFFIXES "" release)
find_library(ASSIMP_LIBRARY_DBG NAMES ${ASSIMP_LIBRARY_NAMES_DBG} HINTS ${ASSIMP_LIBRARY_DIR} PATH_SUFFIXES "" debug)

make_library_set(ASSIMP_LIBRARY)

findpkg_finish(ASSIMP)

set( Target_Inc ${Target_Inc} ${ASSIMP_INCLUDE_DIRS})
set( Target_Lib ${ASSIMP_LIBRARIES})
set( Target_Flags ${Target_Flags} "-DOPENDDL_NO_USE_CPP11 -DASSIMP_BUILD_BOOST_WORKAROUND -DASSIMP_BUILD_NO_C4D_IMPORTER -DASSIMP_BUILD_DLL_EXPORT")
