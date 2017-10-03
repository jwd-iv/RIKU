include_directories(../include)

add_executable(riku_test main.cpp)

target_link_libraries(riku_test RIKU)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set_target_properties(riku_test PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT riku_test)
endif()

add_test(NAME riku_unittest COMMAND riku_test)
