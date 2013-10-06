include_directories(include)
include_directories(${PARTICLEUNIVERSE_INCLUDE_DIRS})

set (ATLASIMAGETOOL_NAME "atlasimagetool")

file(GLOB PROJECT_SOURCES
	src/*.cpp
)

add_executable(${ATLASIMAGETOOL_NAME} ${PROJECT_SOURCES})
target_link_libraries(${ATLASIMAGETOOL_NAME} ${LIBS} ${PARTICLEUNIVERSE_LIBRARIES})

# Copy atlas.cfg to the build folder
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/atlas.cfg ${ParticleUniverse_BINARY_DIR}/atlas.cfg COPYONLY)
