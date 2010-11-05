cmake_minimum_required(VERSION 2.6)

project (mesademos)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)

FIND_LIBRARY( GLEW_glew_LIBRARY GLEW
	/usr/lib
)

FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
      /usr/include/GL
)

add_subdirectory (src)
