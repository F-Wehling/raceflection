#This files finds all files and groups them according to their folder

# search for all header files
file(GLOB_RECURSE All_Header_h "*.h" )
file(GLOB_RECURSE All_Header_hh "*.hh")
file(GLOB_RECURSE All_Header_hpp "*.hpp")

set(All_Header ${All_Header_h} ${All_Header_hh} ${All_Header_hpp})

# search for all source files
file(GLOB_RECURSE All_Source_c "*.c" )
file(GLOB_RECURSE All_Source_cc "*.cc")
file(GLOB_RECURSE All_Source_cpp "*.cpp")

set(All_Header ${All_Header_h} ${All_Header_hh} ${All_Header_hpp})
set(All_Source ${All_Source_c} ${All_Source_cc} ${All_Source_cpp})

foreach( header_file ${All_Header})
		get_filename_component(header_directory ${header_file} DIRECTORY)
	
		#remove the source directory 
		string(LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" to_remove)
		string(SUBSTRING ${header_directory} ${to_remove} -1 header_filter)
		
		#find the first / to find the first folder
		string(FIND ${header_filter} "/" first_folder_pos )		
		
		set(filter "")
		if(NOT ${first_folder_pos} EQUAL -1)		
			#replace the first folder by the filter Header Files
			string(SUBSTRING ${header_filter} ${first_folder_pos} -1 filter )
			
			#prepend Header Files 
			set(filter "Header Files/${filter}")
			
			#replace all remaining / by \\
			string(REPLACE "/" "\\" filter ${filter})
		else()
			set(filter "Header Files")
		endif()
		
		
		#group it into the source group
		source_group("${filter}" FILES ${header_file})	
endforeach()


foreach( source_file ${All_Source})
		get_filename_component(source_directory ${source_file} DIRECTORY)
	
		#remove the source directory 
		string(LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" to_remove)
		string(SUBSTRING ${source_directory} ${to_remove} -1 source_filter)
		
		#find the first / to find the first folder
		string(FIND ${source_filter} "/" first_folder_pos )
		
		set(filter "")
		if(NOT ${first_folder_pos} EQUAL -1)		
			#replace the first folder by the filter Header Files
			string(SUBSTRING ${source_filter} ${first_folder_pos} -1 filter )
			
			#prepend Source Files 
			set(filter "Source Files/${filter}")
			
			#replace all remaining / by \\
			string(REPLACE "/" "\\" filter ${filter})
		else()
			set(filter "Source Files")
		endif()		
		
		#group it into the source group
		source_group("${filter}" FILES ${source_file})	
endforeach()

set(Target_h ${All_Header})
set(Target_src ${All_Source})