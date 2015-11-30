###---
### include the correct boost lib and include files
###
message("\tExternal: Ogre")
include(FindPkgMacros)

set(OGRE_INCLUDE_DIR ${extern_root}/include/OGRE)
set(OGRE_LIBRARY_DIR ${extern_root}/lib)
set(Target_Inc "${Target_Inc} ${OGRE_INCLUDE_DIR}")

set(OGRE_LIBRARY_NAMES OgreMain OgreOverlay )
# v2-1 set(OGRE_LIBRARY_NAMES OgreHlmsPbs OgreHlmsUnlit OgreMain OgreMeshLodGenerator OgreOverlay )

unset(OGRE_LIBRARY_REL CACHE)
unset(OGRE_LIBRARY_DBG CACHE)

foreach( OGRE_LIB_NAME ${OGRE_LIBRARY_NAMES})
	unset(OGRE_LIB_NAME_DBG)
	get_debug_names(OGRE_LIB_NAME)
	unset(OGRE_LIB_REL CACHE)
	find_library(OGRE_LIB_REL NAMES ${OGRE_LIB_NAME} HINTS ${OGRE_LIBRARY_DIR} PATH_SUFFIXES "" release)
	unset(OGRE_LIB_DBG CACHE)
	find_library(OGRE_LIB_DBG NAMES ${OGRE_LIB_NAME_DBG} HINTS ${OGRE_LIBRARY_DIR} PATH_SUFFIXES "" debug)
				
	set(OGRE_LIBRARY_REL ${OGRE_LIBRARY_REL} ${OGRE_LIB_REL})
	set(OGRE_LIBRARY_DBG ${OGRE_LIBRARY_DBG} ${OGRE_LIB_DBG})	
endforeach()
	
make_library_set(OGRE_LIBRARY)

findpkg_finish(OGRE)

set(Target_Lib ${Target_Lib} ${OGRE_LIBRARIES})

#set(Target_Release_Lib ${Target_Release_Lib} ${OGRE_Lib_Release})
#set(Target_Debug_Lib ${Target_Debug_Lib} ${OGRE_Lib_Debug})