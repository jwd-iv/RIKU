include_directories (../include/)

file(GLOB_RECURSE RIKU_src *.cpp)
file(GLOB_RECURSE RIKU_hdr ../include/*.h*)

add_library(riku STATIC ${RIKU_src} ${RIKU_hdr})
set_target_properties(riku PROPERTIES
    OUTPUT_NAME riku
    DEBUG_POSTFIX -d
    FOLDER riku
)
set(install_libs ${install_libs} riku)

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
