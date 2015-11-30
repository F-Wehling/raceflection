#
# Debug/Optimized definitions
#

# define include directories for specific targets
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	if( ${OUTPUTCONFIG} STREQUAL "DEBUG" )
		target_include_directories( ${Target_Name} PRIVATE ${Target_Inc_Debug} )
		# message("${OUTPUTCONFIG} include: ${Target_Inc_Debug}")
	else()
		target_include_directories( ${Target_Name} PRIVATE ${Target_Inc_Release} )
		# message("${OUTPUTCONFIG} include: ${Target_Inc_Release}")
	endif()
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# define target include directories for specific targets
foreach(lib ${Target_Debug_Lib})
	message("\tDEBUG: link ${Target_Name} against ${lib}.")
	target_link_libraries( ${Target_Name} debug ${lib} )
endforeach()

foreach(lib ${Target_Release_Lib})
	message("\tRELEASE: link ${Target_Name} against ${lib}.")
	target_link_libraries( ${Target_Name} optimized ${lib} )
endforeach()