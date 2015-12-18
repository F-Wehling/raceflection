project(nv_dds)

include_directories("./include")

add_library(nv_dds STATIC source/nvDDS.cpp)


install( TARGETS nv_dds${COMPILE_POSTFIX}
	 LIBRARY DESTINATION ${extern_root}/lib/debug CONFIGURATIONS Debug
	 ARCHIVE DESTINATION ${extern_root}/lib/debug CONFIGURATIONS Debug
	 RUNTIME DESTINATION ${extern_root}/bin/debug CONFIGURATIONS Debug
)

install( TARGETS nv_dds${COMPILE_POSTFIX}
	 LIBRARY DESTINATION ${extern_root}/lib/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
	 ARCHIVE DESTINATION ${extern_root}/lib/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
	 RUNTIME DESTINATION ${extern_root}/bin/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
)
	 
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "${extern_root}/include" )