include_directories (../include/)

file(GLOB_RECURSE RIKU_src *.cpp)
file(GLOB_RECURSE RIKU_hdr ../include/*.h*)

add_library(RIKU STATIC ${RIKU_src} ${RIKU_hdr})
set_target_properties(RIKU PROPERTIES
    OUTPUT_NAME RIKU
    DEBUG_POSTFIX -d
    FOLDER RIKU
)
set(install_libs ${install_libs} RIKU)

install(
    DIRECTORY ../include
    DESTINATION .
    FILES_MATCHING PATTERN "**.h*"
)
install(
    TARGETS ${install_libs}
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION bin
)
