#include ( ${cmake_root}/boost.cmake ) # link against boost

#different names for each output configuration
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} UPPERSTROUTPUTCONFIG )
	if("${UPPERSTROUTPUTCONFIG}" STREQUAL "RELEASE")
		set_target_properties( ${Plugin_Target_Name} PROPERTIES OUTPUT_NAME_${UPPERSTROUTPUTCONFIG} "${Plugin_Target_Name}-${OUTPUTCONFIG}")	
	endif()
endforeach()

# define target include directories for specific targets	
#foreach(lib ${Target_Dep})
#	message("\tALL: ${Target_Name} depends on ${lib}")
#	add_dependencies( ${Target_Name} ${lib} )
#endforeach()
#foreach(lib ${Target_Lib})
#	message("\tALL: link ${Target_Name} against ${lib}")
#	target_link_libraries( ${Target_Name} ${lib} )
#endforeach()

if(NOT "${Target_Dep}" STREQUAL "")
	message("Add dependencies: ${Target_Dep}")
	add_dependencies(${Target_Name} ${Target_Dep})
endif()

if(NOT "${Target_Lib}" STREQUAL "")	
	message("Add link: ${Target_Lib}")
	target_link_libraries(${Target_Name} ${Target_Lib})
endif()

include( "${cmake_root}/DebugOptimized.cmake" )

include_directories( include ${Target_Inc} )
target_include_directories( ${Target_Name} PUBLIC include ${Target_Inc} )

set_target_properties( ${Target_Name} PROPERTIES OUTPUT_NAME_DEBUG "${Target_Name}-d")
set_target_properties( ${Target_Name} PROPERTIES OUTPUT_NAME_RELEASE "${Target_Name}")
set_target_properties( ${Target_Name} PROPERTIES OUTPUT_NAME_RELWITHDBGIFNO "${Target_Name}-relwithdbginfo")
set_target_properties( ${Target_Name} PROPERTIES OUTPUT_NAME_MINSIZEREL "${Target_Name}-minsizerel")	

if (WIN32)
	set(Target_Defs ${Target_Defs} -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -DNOMINMAX -DWIN32_LEAN_AND_MEAN -DELPP_WINSOCK2 -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

add_definitions(${Target_Defs})
if(WIN32)
	set(Target_Flags "${Target_Flags} /FI\"Core.h\" /bigobj") #/FI\"Logging.h\" 
elseif(UNIX)
	set(Target_Flags "${Target_Flags} -include\"Core.h\" -std=c++11") #/FI\"Logging.h\" 
endif()
set_target_properties(${Target_Name} PROPERTIES COMPILE_FLAGS ${Target_Flags})
set_target_properties(${Target_Name} PROPERTIES COMPILE_FLAGS_RELEASE "/MP")
