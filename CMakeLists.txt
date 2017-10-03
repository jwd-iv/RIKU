cmake_minimum_required(VERSION 3.3)
enable_testing()

project(RIKU)

option(RIKU_TEST "Build RIKU test executable" ON)
option(RIKU_DOCS "Generate RIKU documentation" ON)

include_directories(include)
add_subdirectory(src)
if(RIKU_TEST)
  add_subdirectory(test)
endif()

if(RIKU_DOCS)
  add_subdirectory(docs)
endif()
