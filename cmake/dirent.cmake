###---
### include the correct boost lib and include files
###
if(WIN32)
	message("\tExternal: Dirent for Win32")

	set(DIRENT_INCLUDE_DIR ${extern_root}/include/)
	set(Target_Inc "${Target_Inc} ${DIRENT_INCLUDE_DIR}")
endif()