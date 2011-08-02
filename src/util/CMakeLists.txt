include_directories(
	${CMAKE_SOURCE_DIR}/src/util
	${GLEXT_INCLUDE_DIR}
	${OPENGL_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

add_library (util
	readtex.c
	shaderutil.c
	showbuffer.c
	trackball.c
)
