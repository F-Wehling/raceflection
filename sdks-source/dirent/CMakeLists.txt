
project(dirent)

if(WIN32)
    install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "${extern_root}/include" )
endif()
