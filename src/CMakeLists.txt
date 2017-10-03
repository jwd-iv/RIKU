include_directories (../include/)

file(GLOB_RECURSE RIKU_src *.cpp)
file(GLOB_RECURSE RIKU_hdr ../include/*.h*)

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
