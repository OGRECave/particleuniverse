set(PROJECT_NAME "ParticleUniverse")

project(${PROJECT_NAME} CXX C)
cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Avoid source tree pollution
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

# We want the binaries to be easily accessible
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Append _d for debug builds
set(CMAKE_DEBUG_POSTFIX "_d")

# Add the libraries
# OGRE
find_package(OGRE 1.11 REQUIRED)
link_directories(${OGRE_LIBRARY_DIRS})
include_directories(${OGRE_INCLUDE_DIRS})
list(APPEND LIBS ${OGRE_LIBRARIES})

add_subdirectory(Plugins/ParticleUniverse)

# Samples and tools
option(PU_BUILD_SAMPLES "Build plugin samples" OFF)
option(PU_BUILD_TOOLS "Build tools" OFF)
option(PU_BUILD_EDITOR "Build PUED Editor [DOES NOT RUN]" OFF)

if(PU_BUILD_SAMPLES)
  add_subdirectory(Samples/ParticleUniverseDemo)
endif()

if(PU_BUILD_TOOLS)
  add_subdirectory(Tools/AtlasImageTool)
endif()

# doxygen stuff
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # prepare doxygen configuration file
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Plugins/ParticleUniverse/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  add_custom_target( docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen."
    VERBATIM
  )

  # cleanup $build/api-doc on "make clean"
  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES docs)
endif (DOXYGEN_FOUND)

if(PU_BUILD_EDITOR)
  # does not use Bites, needs the cfg
  file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  
  file(COPY PUED/pued.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

  # wxWidgets
  find_package(wxWidgets REQUIRED COMPONENTS core base propgrid adv)
  include(${wxWidgets_USE_FILE})

  file(GLOB_RECURSE pued_SRC PUED/src/*.cpp)

  # add the source files as usual
  add_executable(PUED ${pued_SRC})
  target_include_directories(PUED PRIVATE PUED/include)
  target_link_libraries(PUED PRIVATE OgreMain ${wxWidgets_LIBRARIES} ParticleUniverse)

  if(UNIX)
    # we need GTK because wxWidgets is crap
    find_package(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK REQUIRED gtk+-3.0)

    target_compile_options(PUED PRIVATE ${GTK_CFLAGS_OTHER})
    target_include_directories(PUED PRIVATE ${GTK_INCLUDE_DIRS})
    target_link_libraries(PUED PRIVATE ${GTK_LIBRARIES})
  endif()
endif()