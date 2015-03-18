set(PROJECT_NAME "ParticleUniverse")

project(${PROJECT_NAME} CXX C)
cmake_minimum_required(VERSION 2.8)

# Avoid source tree pollution
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

# Include path for additional CMake library finding scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# We want the binaries to be easily accessible
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Append _d for debug builds
set(CMAKE_DEBUG_POSTFIX "_d")

# Add the libraries
# OGRE
find_package(OGRE REQUIRED)
link_directories(${OGRE_LIBRARY_DIRS})
include_directories(${OGRE_INCLUDE_DIRS})
list(APPEND LIBS ${OGRE_LIBRARIES})

add_definitions(-DOGRE_PLUGIN_DIR_REL="${OGRE_PLUGIN_DIR_REL}")
add_definitions(-DOGRE_PLUGIN_DIR_DBG="${OGRE_PLUGIN_DIR_DBG}")

add_subdirectory(Plugins/ParticleUniverse)

# Samples and tools
set(PU_BUILD_SAMPLES ON CACHE BOOL "Build plugin samples")
set(PU_BUILD_TOOLS ON CACHE BOOL "Build tools")

if(PU_BUILD_SAMPLES)
  add_subdirectory(Samples/ParticleUniverseDemo)
endif()

if(PU_BUILD_TOOLS)
  add_subdirectory(Tools/AtlasImageTool)
endif()

