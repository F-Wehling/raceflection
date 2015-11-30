###---
### include the correct glfw lib and include files
###
message("\tExternal: GLFW")
include(FindPkgMacros)

set(GLFW_INCLUDE_DIR ${extern_root}/include/)
set(GLFW_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc ${Target_Inc} ${GLFW_INCLUDE_DIR})

set(GLFW_LIBRARY_NAMES glfw glfw3)

unset(GLFW_LIBRARY_REL CACHE)
unset(GLFW_LIBRARY_DBG CACHE)

get_debug_names(GLFW_LIBRARY_NAMES)

find_library(GLFW_LIBRARY_REL NAMES ${GLFW_LIBRARY_NAMES} HINTS ${GLFW_LIBRARY_DIR} PATH_SUFFIXES "" release)
find_library(GLFW_LIBRARY_DBG NAMES ${GLFW_LIBRARY_NAMES_DBG} HINTS ${GLFW_LIBRARY_DIR} PATH_SUFFIXES "" debug)


#foreach( GLFW_LIB_NAME ${GLFW_LIBRARY_NAMES})
#	unset(GLFW_LIB_NAME_DBG)
#	get_debug_names(GLFW_LIB_NAME)
#	unset(GLFW_LIB_REL CACHE)
#	find_library(GLFW_LIB_REL NAMES ${GLFW_LIB_NAME} HINTS ${GLFW_LIBRARY_DIR} PATH_SUFFIXES "" release)
#	unset(GLFW_LIB_DBG CACHE)
#	find_library(GLFW_LIB_DBG NAMES ${GLFW_LIB_NAME_DBG} HINTS ${GLFW_LIBRARY_DIR} PATH_SUFFIXES "" debug)
		
#	set(GLFW_LIBRARY_REL ${GLFW_LIBRARY_REL} ${GLFW_LIB_REL})
#	set(GLFW_LIBRARY_DBG ${GLFW_LIBRARY_DBG} ${GLFW_LIB_DBG})	
#endforeach()
	
make_library_set(GLFW_LIBRARY)

findpkg_finish(GLFW)

set(Target_Lib ${Target_Lib} ${GLFW_LIBRARIES})
