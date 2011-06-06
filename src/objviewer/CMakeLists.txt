include_directories (
	${CMAKE_SOURCE_DIR}/src/util
	${OPENGL_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_directories (
	${CMAKE_SOURCE_DIR}/src/util
)

link_libraries (
	util
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLUT_glut_LIBRARY}
	${GLEW_glew_LIBRARY}
)


add_executable (objview
	glm.c
	glmdraw.c
	objview.c
	skybox.c
)
install (TARGETS objview DESTINATION objviewer)

file (GLOB data *.mtl *.obj *.rgb)

install (FILES ${data} DESTINATION objviewer)
