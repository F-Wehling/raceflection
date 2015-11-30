###---
### include the correct boost lib and include files
###
message("\tExternal: ACGL")
include(FindPkgMacros)

set(ACGL_INCLUDE_DIR ${extern_root}/include/)
set(ACGL_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc ${Target_Inc} ${ACGL_INCLUDE_DIR})

set(ACGL_LIBRARY_NAMES ACGL)

unset(ACGL_LIBRARY_REL CACHE)
unset(ACGL_LIBRARY_DBG CACHE)

foreach( ACGL_LIB_NAME ${ACGL_LIBRARY_NAMES})
	unset(ACGL_LIB_NAME_DBG)
	get_debug_names(ACGL_LIB_NAME)
	unset(ACGL_LIB_REL CACHE)
	find_library(ACGL_LIB_REL NAMES ${ACGL_LIB_NAME} HINTS ${ACGL_LIBRARY_DIR} PATH_SUFFIXES "" release)
	unset(ACGL_LIB_DBG CACHE)
	find_library(ACGL_LIB_DBG NAMES ${ACGL_LIB_NAME_DBG} HINTS ${ACGL_LIBRARY_DIR} PATH_SUFFIXES "" debug)
		
	set(ACGL_LIBRARY_REL ${ACGL_LIBRARY_REL} ${ACGL_LIB_REL})
	set(ACGL_LIBRARY_DBG ${ACGL_LIBRARY_DBG} ${ACGL_LIB_DBG})	
endforeach()
	
make_library_set(ACGL_LIBRARY)

findpkg_finish(ACGL)

# this is very bad
if(UNIX AND "$ENV{USER}" STREQUAL "florian")
        set(Target_Defs ${Target_Defs} -DACGL_OPENGL_PROFILE_CORE -DACGL_OPENGL_VERSION_33)
else()
    set(Target_Defs ${Target_Defs} -DACGL_OPENGL_PROFILE_CORE -DACGL_OPENGL_VERSION_42)
endif()

set(Target_Lib ${Target_Lib} ${ACGL_LIBRARIES})
