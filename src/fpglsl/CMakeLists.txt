include_directories (
	${mesademos_SOURCE_DIR}/src/util
	${OPENGL_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_directories (
	${mesademos_SOURCE_DIR}/src/util
)

link_libraries (
	util
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLUT_glut_LIBRARY}
	${GLEW_glew_LIBRARY}
)

set (targets
	fp-tri
)

foreach (target ${targets})
	add_executable (${target} ${target}.c)
endforeach (target)

install (TARGETS ${targets} DESTINATION fpglsl)

file (GLOB data *.glsl)

install (FILES ${data} DESTINATION fpglsl)
