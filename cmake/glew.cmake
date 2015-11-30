###---
### include the correct glew lib and include files
###
message("\tExternal: Glew")

set(glew_inc_dir "${extern_root}/glew/include/")
set(glew_lib_dir "${extern_root}/glew/lib/")

set( Target_Inc ${Target_Inc} ${glew_inc_dir})
set( Target_Flags ${Target_Flags} "-DGLEW_STATIC")

if(WIN32)
	set(Target_Debug_Lib ${Target_Debug_Lib} ${glew_lib_dir}glew32sd.lib)
	set(Target_Release_Lib ${Target_Release_Lib} ${glew_lib_dir}glew32s.lib)
	set(Target_Lib ${Target_Lib})
elseif(UNIX)
	message("\tError: GLEW LIBRARY NOT AVAILABLE.")
endif()