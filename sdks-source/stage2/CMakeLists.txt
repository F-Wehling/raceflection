cmake_minimum_required(VERSION 3.0.0)
include(../../cmake/preproject.cmake)

set(stage1_install "${extern_root}/stage1-intermed")
if(NOT EXISTS ${stage1_install})
	message("Make sure you INSTALLED stage1 before building stage 2")
else()

set(ProjectDep_Name "RacingGame-Dependencies (Stage 2)")
project(${ProjectDep_Name})
message("----------------------------")
message("---SOLUTION: ${ProjectDep_Name}---")
message("----------------------------")
message("")

set(CMAKE_PREFIX_PATH ${stage1_install} ${CMAKE_PREFIX_PATH})
set(CMAKE_MODULE_PATH ${cmake_root} ${CMAKE_MODULE_PATH})

set(CMAKE_INSTALL_PREFIX ${extern_root})

#add_subdirectory("ogre/")
add_subdirectory("assimp/")
add_subdirectory("acgl/")
add_subdirectory("glfw/")

# install stage 1 dependencies

set(DEP_DIR ${stage1_install})

set(INSTALL_DEPENDENCIES TRUE)
set(COPY_DEPENDENCIES TRUE)

macro(install_debug INPUT)
  if (EXISTS ${DEP_DIR}/bin/debug/${INPUT})
    if (IS_DIRECTORY ${DEP_DIR}/bin/debug/${INPUT})
      install(DIRECTORY ${DEP_DIR}/bin/debug/${INPUT} DESTINATION bin/debug CONFIGURATIONS Debug)
    else ()
      install(FILES ${DEP_DIR}/bin/debug/${INPUT} DESTINATION bin/debug CONFIGURATIONS Debug)
    endif ()
  else()
    message(send_error "${DEP_DIR}/bin/debug/${INPUT} did not exist, can't install!")
  endif ()
endmacro()

macro(install_release INPUT)
  if (EXISTS ${DEP_DIR}/bin/release/${INPUT})
    if (IS_DIRECTORY ${DEP_DIR}/bin/release/${INPUT})
      install(DIRECTORY ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/release CONFIGURATIONS Release None "")
      install(DIRECTORY ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo)
      install(DIRECTORY ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel)
    else ()
      install(FILES ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/release CONFIGURATIONS Release None "")
      install(FILES ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo)
      install(FILES ${DEP_DIR}/bin/release/${INPUT} DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel)
    endif ()
  else()
    message(send_error "${DEP_DIR}/bin/release/${INPUT} did not exist, can't install!")
  endif ()
endmacro()

macro(copy_debug INPUT)
  if (EXISTS ${DEP_DIR}/lib/debug/${INPUT})
    if (MINGW OR NMAKE)
      configure_file(${DEP_DIR}/lib/debug/${INPUT} ${BINARY_DIR}/lib/${INPUT} COPYONLY)
	else ()
      if (IS_DIRECTORY ${DEP_DIR}/lib/debug/${INPUT})
        install(DIRECTORY ${DEP_DIR}/lib/debug/${INPUT} DESTINATION lib/debug)
      else ()
        configure_file(${DEP_DIR}/lib/debug/${INPUT} ${BINARY_DIR}/lib/debug/${INPUT} COPYONLY)
      endif ()
	endif ()
  endif ()
endmacro()

macro(copy_release INPUT)
  if (EXISTS ${DEP_DIR}/lib/release/${INPUT})
    if (MINGW OR NMAKE)
      configure_file(${DEP_DIR}/lib/release/${INPUT} ${BINARY_DIR}/lib/${INPUT} COPYONLY)
	else ()
      if (IS_DIRECTORY ${DEP_DIR}/lib/release/${INPUT})
        install(DIRECTORY ${DEP_DIR}/lib/release/${INPUT} DESTINATION lib/release CONFIGURATIONS Release None "")
        install(DIRECTORY ${DEP_DIR}/lib/release/${INPUT} DESTINATION lib/relwithdebinfo CONFIGURATIONS RelWithDebInfo)
        install(DIRECTORY ${DEP_DIR}/lib/release/${INPUT} DESTINATION lib/minsizerel CONFIGURATIONS MinSizeRel)
      else ()
        configure_file(${DEP_DIR}/lib/release/${INPUT} ${BINARY_DIR}/lib/release/${INPUT} COPYONLY)
        configure_file(${DEP_DIR}/lib/release/${INPUT} ${BINARY_DIR}/lib/relwithdebinfo/${INPUT} COPYONLY)
        configure_file(${DEP_DIR}/lib/release/${INPUT} ${BINARY_DIR}/lib/minsizerel/${INPUT} COPYONLY)
      endif ()
	endif ()
  endif ()
endmacro ()

if (EXISTS ${DEP_DIR}/include/)
  install(DIRECTORY ${DEP_DIR}/include/ DESTINATION include)
endif ()

if (EXISTS ${DEP_DIR}/lib/)
	install(DIRECTORY ${DEP_DIR}/lib/ DESTINATION lib)
endif ()

if (EXISTS ${DEP_DIR}/include/OIS/)
	install(DIRECTORY ${DEP_DIR}/include/OIS   DESTINATION include)
endif ()

if(WIN32)
	if (EXISTS ${DEP_DIR}/lib/debug/OIS_d.lib)
	  install(FILES
		${DEP_DIR}/lib/debug/OIS_d.lib
		DESTINATION lib/debug CONFIGURATIONS Debug
	  )
	endif ()

	if (EXISTS ${DEP_DIR}/lib/release/OIS.lib)
	  install(FILES
		${DEP_DIR}/lib/release/OIS.lib
		DESTINATION lib/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
	  )
	endif ()

elseif(UNIX)
	if (EXISTS ${DEP_DIR}/lib/libOIS_d.so)
	  install(FILES
		${DEP_DIR}/lib/libOIS_d.so
		DESTINATION lib/debug CONFIGURATIONS Debug
	  )
	endif ()

	message("Check for ${DEP_DIR}/lib/libOIS.so")
	if (EXISTS ${DEP_DIR}/lib/libOIS.so)
	  message("Install OIS.so")
	  install(FILES
		${DEP_DIR}/lib/libOIS.so
		DESTINATION lib/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
	  )
	endif ()
endif()

  if (MINGW)
	if (EXISTS ${OIS_LIBRARY_DBG})
		install(FILES ${OIS_LIBRARY_DBG} DESTINATION lib/debug CONFIGURATIONS Debug)
	else()	
		install(FILES DESTINATION lib/debug CONFIGURATIONS Debug)
	endif ()
	if (EXISTS ${OIS_LIBRARY_REL})
		install(FILES ${OIS_LIBRARY_REL} DESTINATION lib/relwithdebinfo CONFIGURATIONS RelWithDebInfo)
		install(FILES ${OIS_LIBRARY_REL} DESTINATION lib/release CONFIGURATIONS Release)
		install(FILES ${OIS_LIBRARY_REL} DESTINATION lib/minsizerel CONFIGURATIONS MinSizeRel)
	else()
		install(FILES DESTINATION lib/relwithdebinfo CONFIGURATIONS RelWithDebInfo)
		install(FILES DESTINATION lib/release CONFIGURATIONS Release)
		install(FILES DESTINATION lib/minsizerel CONFIGURATIONS MinSizeRel)
	endif ()
  endif () # MINGW

if(WIN32)
	# copy the dependency DLLs to the right places
	if(NOT (WINDOWS_STORE OR WINDOWS_PHONE))
	  install_debug(OIS_d.dll)
	  install_release(OIS.dll)
	endif ()
endif () # WIN32

if (COPY_DEPENDENCIES)
	# copy the required Libraries to the build directory (configure_file is the only copy-like op I found in CMake)
	if( (BUILD_SAMPLES OR BUILD_TESTS) AND NOT (WINDOWS_STORE OR WINDOWS_PHONE) )
		if(EXISTS ${OIS_BINARY_DBG} AND EXISTS ${OIS_BINARY_REL})
		  file(COPY ${OIS_BINARY_DBG} DESTINATION ${BINARY_DIR}/bin/debug)
		  file(COPY ${OIS_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/release)
		  file(COPY ${OIS_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/relwithdebinfo)
		  file(COPY ${OIS_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/minsizerel)
		endif()
	endif()

	if (BUILD_PLUGIN_CG)
	  # if MinGW or NMake, the release/debug cg.dll's would conflict, so just pick one
	  if (MINGW OR (CMAKE_GENERATOR STREQUAL "NMake Makefiles"))
		if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		  file(COPY ${Cg_BINARY_DBG} DESTINATION ${BINARY_DIR}/bin/debug)
		else ()
		  file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/release)
				file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/relwithdebinfo)
				file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/minsizerel)
		endif ()
	  else ()
		if(EXISTS ${Cg_BINARY_DBG} AND EXISTS ${Cg_BINARY_REL})
			file(COPY ${Cg_BINARY_DBG} DESTINATION ${BINARY_DIR}/bin/debug)
			file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/release)
			file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/relwithdebinfo)
			file(COPY ${Cg_BINARY_REL} DESTINATION ${BINARY_DIR}/bin/minsizerel)
		endif()
	  endif ()
	endif()
endif ()

endif() #end check-stage1 compiled
