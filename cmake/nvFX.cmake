###---
### include the correct boost lib and include files
###
message("\tExternal: nvFX")
include(FindPkgMacros)

set(NVFX_INCLUDE_DIR ${extern_root}/include/)
set(NVFX_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc ${Target_Inc} ${NVFX_INCLUDE_DIR})

set(NVFX_LIBRARY_NAMES FxLib FxLibGL FxParser nvFxDep)

unset(NVFX_LIBRARY_REL CACHE)
unset(NVFX_LIBRARY_DBG CACHE)

foreach( NVFX_LIB_NAME ${NVFX_LIBRARY_NAMES})
	unset(NVFX_LIB_NAME_DBG)
	get_debug_names(NVFX_LIB_NAME)
	unset(NVFX_LIB_REL CACHE)
	find_library(NVFX_LIB_REL NAMES ${NVFX_LIB_NAME} HINTS ${NVFX_LIBRARY_DIR} PATH_SUFFIXES "" release)
	unset(NVFX_LIB_DBG CACHE)
	find_library(NVFX_LIB_DBG NAMES ${NVFX_LIB_NAME_DBG} HINTS ${NVFX_LIBRARY_DIR} PATH_SUFFIXES "" debug)
		
	set(NVFX_LIBRARY_REL ${NVFX_LIBRARY_REL} optimized;${NVFX_LIB_REL})
	set(NVFX_LIBRARY_DBG ${NVFX_LIBRARY_DBG} debug;${NVFX_LIB_DBG})	
endforeach()
	
make_library_set(NVFX_LIBRARY)

findpkg_finish(NVFX)

set(Target_Lib ${Target_Lib} ${NVFX_LIBRARIES})
