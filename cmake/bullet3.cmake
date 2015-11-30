###---
### include the correct boost lib and include files
###
message("\tExternal: Bullet3")
include(FindPkgMacros)

set(BULLET3_INCLUDE_DIR ${extern_root}/include/bullet/)
set(BULLET3_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc ${Target_Inc} ${BULLET3_INCLUDE_DIR})

set(BULLET3_LIBRARY_NAMES BulletCollision BulletDynamics LinearMath)

unset(BULLET3_LIBRARY_REL CACHE)
unset(BULLET3_LIBRARY_DBG CACHE)

foreach( BULLET3_LIB_NAME ${BULLET3_LIBRARY_NAMES})
	unset(BULLET3_LIB_NAME_DBG)
	get_debug_names(BULLET3_LIB_NAME)
	unset(BULLET3_LIB_REL CACHE)
	find_library(BULLET3_LIB_REL NAMES ${BULLET3_LIB_NAME} HINTS ${BULLET3_LIBRARY_DIR} PATH_SUFFIXES "" release)
	unset(BULLET3_LIB_DBG CACHE)
	find_library(BULLET3_LIB_DBG NAMES ${BULLET3_LIB_NAME_DBG} HINTS ${BULLET3_LIBRARY_DIR} PATH_SUFFIXES "" debug)
		
	set(BULLET3_LIBRARY_REL ${BULLET3_LIBRARY_REL} ${BULLET3_LIB_REL})
	set(BULLET3_LIBRARY_DBG ${BULLET3_LIBRARY_DBG} ${BULLET3_LIB_DBG})	
endforeach()
	
make_library_set(BULLET3_LIBRARY)

findpkg_finish(BULLET3)

set(Target_Lib ${Target_Lib} ${BULLET3_LIBRARIES})
