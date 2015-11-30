###---
### include the correct hash lib and include files
###
message("\tExternal: Hash")

set(hash_inc_dir "${extern_root}/hash-library/include/")
set(hash_lib_dir "${extern_root}/hash-library/lib/")

set( Target_Inc ${Target_Inc} ${hash_inc_dir})

if(WIN32)
	set(Target_Debug_Lib ${Target_Debug_Lib} ${hash_lib_dir}hash-library-d.lib)
	set(Target_Release_Lib ${Target_Release_Lib} ${hash_lib_dir}hash-library.lib)
	set(Target_Lib ${Target_Lib})
elseif(UNIX)
	message("\tError: HASH LIBRARY NOT AVAILABLE.")
endif()