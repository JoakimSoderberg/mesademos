cmake_minimum_required (VERSION 2.6)

# Allow targets with duplicate names
cmake_policy(SET CMP0002 OLD)

project (mesademos)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
find_package (X11)

find_library (GLEW_glew_LIBRARY GLEW
	/usr/lib
)

find_path (GLEW_INCLUDE_DIR GL/glew.h
      /usr/include/GL
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	link_libraries (winmm)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

add_subdirectory (src)
