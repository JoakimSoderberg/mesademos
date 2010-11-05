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
	accum
	bitmap1
	bitmap2
	blendeq
	blendxor
	copy
	cursor
	depth
	eval
	fog
	font
	line
	logo
	nurb
	oglinfo
	olympic
	overlay
	point
	prim
	quad
	rgbtoppm
	select
	shape
	sphere
	star
	stencil
	stretch
	texture
	tri
	wave
)

foreach (target ${targets})
	add_executable (${target} ${target}.c)
endforeach (target)

install (TARGETS ${targets} DESTINATION samples)
