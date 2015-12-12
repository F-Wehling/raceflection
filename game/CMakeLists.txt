cmake_minimum_required(VERSION 3.0.0)
include(../cmake/preproject.cmake)

#The racing game gets an own solution
set(Project_Name "RacingGame")
project(${Project_Name})
message("----------------------------")
message("---SOLUTION: ${Project_Name}---")
message("----------------------------")
message("")

# it may consist of several applications...
file(GLOB_RECURSE Application_Projects ${app_root}/*/CMakeLists.txt)
#... shared libraries
file(GLOB_RECURSE Shared_Projects ${shared_root}/*/CMakeLists.txt)
#... and static libraries (to ensure correct linkage order, the static projects are defined with names)
set(Static_Projects	Core Resource)
foreach(static_lib ${Static_Projects})
	add_subdirectory(${static_root}/${static_lib})
endforeach()

foreach(shared_lib ${Shared_Projects})
	string(REPLACE "/CMakeLists.txt" "" lib_folder "${shared_lib}")
	string(REPLACE "${shared_root}/" "" lib_name "${lib_folder}")
	set(Shared_${lib_name} true CACHE BOOL "Compile the ${lib_name} Shared-Library")
	if(Shared_${lib_name})
		add_subdirectory(${lib_folder})
	endif()
endforeach()

foreach(app ${Application_Projects})
	string(REPLACE "/CMakeLists.txt" "" app_folder "${app}")
	string(REPLACE "${app_root}/" "" app_name "${app_folder}")
	
	add_subdirectory(${app_folder})
endforeach()
