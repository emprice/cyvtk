cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

PROJECT (cyvtk)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(.)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_custom_command(OUTPUT cyvtk.cpp COMMAND cython --cplus -2 --fast-fail cyvtk.pyx MAIN_DEPENDENCY cyvtk.pyx)
add_library(cyvtk SHARED cyvtk.cpp grid.cc structuredGrid.cc)
set_target_properties(cyvtk PROPERTIES PREFIX "")
target_link_libraries(cyvtk ${VTK_LIBRARIES})
include_directories("/usr/include/python2.7")
