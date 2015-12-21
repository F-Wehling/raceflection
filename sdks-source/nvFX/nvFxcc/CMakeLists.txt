project(nvFxcc)

set(PLATFORM_LIBRARIES
    ${NVFX_LIBS}
    ${OPENGL_LIBRARY}
    ${CUDA_LIB}
    ${NSIGHT_LIB}
)


include_directories(
  "${nvFx_SOURCE_DIR}/include"
    ${GLEW_INCLUDE_DIR}
)

set(PUBLIC_HEADERS
  ${nvFx_SOURCE_DIR}/include/FxLib.h
  ${nvFx_SOURCE_DIR}/include/FxLibEx.h
  ${nvFx_SOURCE_DIR}/include/FxParser.h
)

set(HEADERS
  stdafx.h
)

#####################################################################################
# Glew : source or lib
if(GLEW_SOURCE)
  message(STATUS "found Glew source code. Using it instead of library")
  LIST(APPEND SOURCE_FILES ${GLEW_SOURCE})
  add_definitions(-DGLEW_STATIC)
else()
    message(STATUS "using GLEW library")
    LIST(APPEND PLATFORM_LIBRARIES ${GLEW_LIBRARY})
endif()

#let cmake determine linker language
set(SOURCE_FILES_LIB ${SOURCE_FILES})
LIST(APPEND SOURCE_FILES nvFXcc.cpp )

source_group(sources FILES ${SOURCE_FILES})
source_group("" FILES ${PUBLIC_HEADERS})

# build a static library
add_library(nvFXdep STATIC ${SOURCE_FILES_LIB})
set_target_properties( nvFXdep PROPERTIES FOLDER "nvFXdep" )
set_target_properties( nvFXdep PROPERTIES DEBUG_OUTPUT_NAME "nvFXdep" )
set_target_properties( nvFXdep PROPERTIES RELEASE_OUTPUT_NAME "nvFXdep" )

if(WIN32)
add_executable(nvFxcc
  ${PUBLIC_HEADERS}
  ${HEADERS}
  ${SOURCE_FILES}
)
#linkage dependencies would only be for a SHARED...
target_link_libraries( nvFxcc ${PLATFORM_LIBRARIES} )

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_property
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:IMPORTED_LINK_INTERFACE_LIBRARIES
#set_property(TARGET nvFx PROPERTY LINK_INTERFACE_LIBRARIES "")

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_target_properties
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:FOLDER

set_target_properties( nvFxcc PROPERTIES FOLDER "nvFxcc" )

endif()

if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS nvFXdep
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}/debug" CONFIGURATIONS Debug
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}/debug" CONFIGURATIONS Debug
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}/debug" CONFIGURATIONS Debug )
		
    install(TARGETS nvFXdep
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}/release" CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}/release" CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}/release" CONFIGURATIONS Release RelWithDebInfo MinSizeRel None "" )
  endif()
endif()