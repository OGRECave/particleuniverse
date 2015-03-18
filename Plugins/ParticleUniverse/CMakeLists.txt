# The project's include directories
include_directories(include)

set (PARTICLEUNIVERSE_PLUGIN_NAME "Plugin_ParticleUniverse")

# The project's sources
file(GLOB PROJECT_SOURCES
	src/*.cpp
	src/*/*.cpp
)

set (PARTICLEUNIVERSE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set (PARTICLEUNIVERSE_LIBRARIES ${PARTICLEUNIVERSE_PLUGIN_NAME} PARENT_SCOPE)

# Default to building a shared library rather than static
if(NOT DEFINED BUILD_SHARED_LIBS)
	set(BUILD_SHARED_LIBS ON)
endif()

# When building, we want to export
add_definitions(-DPARTICLE_UNIVERSE_EXPORTS)

# Add library
add_library(${PARTICLEUNIVERSE_PLUGIN_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PARTICLEUNIVERSE_PLUGIN_NAME} ${LIBS})
set_target_properties(${PARTICLEUNIVERSE_PLUGIN_NAME} PROPERTIES
    VERSION "1.9.0"
    SOVERSION "1.9.0"
    PREFIX "")

# Build as a framework on OS X
if(APPLE)
    set_target_properties(${PARTICLEUNIVERSE_PLUGIN_NAME} PROPERTIES FRAMEWORK ON)
endif()

# Set installation options
install(TARGETS ${PARTICLEUNIVERSE_PLUGIN_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	FRAMEWORK DESTINATION lib)
install(DIRECTORY include DESTINATION .)
install(DIRECTORY media DESTINATION .)

# Install debug symbols on windows
if(WIN32)
	if(MSVC)
		install(FILES "${PROJECT_BINARY_DIR}/Debug/${PARTICLEUNIVERSE_PLUGIN_NAME}_d.pdb"
			DESTINATION bin/
			CONFIGURATIONS Debug
		)
		install(FILES "${PROJECT_BINARY_DIR}/RelWithDebInfo/${PARTICLEUNIVERSE_PLUGIN_NAME}.pdb"
			DESTINATION bin/
			CONFIGURATIONS RelWithDebInfo
		)
	endif(MSVC)
endif(WIN32)
