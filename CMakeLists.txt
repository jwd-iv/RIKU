cmake_minimum_required(VERSION 3.3)
#enable_testing()

project(RIKU)

option(RIKU_TEST "Build RIKU test executable" ON)
option(RIKU_DOCS "Generate RIKU documentation" ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../install CACHE PATH "default install path" FORCE)
endif()

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib)
endif()
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

include_directories(include/)
add_subdirectory(src)
if(RIKU_TEST)
  add_subdirectory(test)
endif()

if(RIKU_DOCS)
  add_subdirectory(docs)
endif()
