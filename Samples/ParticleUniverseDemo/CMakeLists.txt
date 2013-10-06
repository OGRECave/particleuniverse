include_directories(include)
include_directories(${PARTICLEUNIVERSE_INCLUDE_DIRS})

set (PARTICLEUNIVERSE_DEMO_NAME "particleuniversedemo")

file(GLOB PROJECT_SOURCES
	src/*.cpp
)

# OIS
find_package(OIS REQUIRED)
include_directories(${OIS_INCLUDE_DIRS})

add_executable(${PARTICLEUNIVERSE_DEMO_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PARTICLEUNIVERSE_DEMO_NAME} ${LIBS} ${OIS_LIBRARIES} ${PARTICLEUNIVERSE_LIBRARIES})

# Copy resources.cfg to the build folder
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg ${ParticleUniverse_BINARY_DIR}/resources.cfg COPYONLY)
