cmake_minimum_required(VERSION 3.6.0)
project(Raytracing)

set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

SET(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
SET(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

INCLUDE_DIRECTORIES( ${INCLUDE_DIR})

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
