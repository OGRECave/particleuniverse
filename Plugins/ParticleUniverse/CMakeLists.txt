# The project's include directories
include_directories(include)

# The project's sources
file(GLOB PROJECT_SOURCES
	src/*.cpp
	src/*/*.cpp
)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
