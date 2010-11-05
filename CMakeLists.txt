cmake_minimum_required (VERSION 2.6)

project (mesademos)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)

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
