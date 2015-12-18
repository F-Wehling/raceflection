###---
### include the correct boost lib and include files
###
message("\tExternal: nv_dds")
include(FindPkgMacros)

set(NVDDS_INCLUDE_DIR ${extern_root}/include/)
set(NVDDS_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc ${Target_Inc} ${NVDDS_INCLUDE_DIR})

set(NVDDS_LIBRARY_NAMES nv_dds libnv_dds)
get_debug_names(NVDDS_LIBRARY_NAMES)

find_library(NVDDS_LIBRARY_REL NAMES ${NVDDS_LIBRARY_NAMES} HINTS ${NVDDS_LIBRARY_DIR} PATH_SUFFIXES "" release Release)
find_library(NVDDS_LIBRARY_DBG NAMES ${NVDDS_LIBRARY_NAMES_DBG} HINTS ${NVDDS_LIBRARY_DIR} PATH_SUFFIXES "" debug Debug)

make_library_set(NVDDS_LIBRARY)

findpkg_finish(NVDDS)

set(Target_Lib ${Target_Lib} ${NVDDS_LIBRARY})
