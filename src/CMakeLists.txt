include_directories (../include/)

file(GLOB_RECURSE RIKU_src *.cpp)
file(GLOB_RECURSE RIKU_hdr ../include/*.h*)

if(MSVC)
	#add a natvis file for better debug support in visual studio.
	set_source_files_properties(../msvc/RIKU.natvis PROPERTIES HEADER_FILE_ONLY TRUE)
	set(RIKU_src ../msvc/RIKU.natvis ${RIKU_src})
endif(MSVC)

add_library(RIKU STATIC ${RIKU_src} ${RIKU_hdr})
set_target_properties(RIKU PROPERTIES
    OUTPUT_NAME RIKU
    DEBUG_POSTFIX -d
    FOLDER RIKU
)

install(
    DIRECTORY ${RIKU_SOURCE_DIR}/include
    DESTINATION include
)
install(
    TARGETS RIKU
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
