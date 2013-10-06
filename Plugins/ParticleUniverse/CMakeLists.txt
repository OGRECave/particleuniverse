# The project's include directories
include_directories(include)

set (PARTICLEUNIVERSE_PLUGIN_NAME "Plugin_ParticleUniverse")

# The project's sources
file(GLOB PROJECT_SOURCES
	src/*.cpp
	src/*/*.cpp
)

add_library(${PARTICLEUNIVERSE_PLUGIN_NAME} SHARED ${PROJECT_SOURCES})
target_link_libraries(${PARTICLEUNIVERSE_PLUGIN_NAME} ${LIBS})
