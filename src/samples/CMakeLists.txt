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
)

set (subdir samples)

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
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	target_link_libraries (${subdir}_${target}
		${OPENGL_gl_LIBRARY}
		${OPENGL_glu_LIBRARY}
		${GLUT_glut_LIBRARY}
		${GLEW_glew_LIBRARY}
	)
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)

if (X11_FOUND)
	include_directories (
		${X11_INCLUDE_DIR}
	)

	set (targets
		oglinfo
	)

	foreach (target ${targets})
		add_executable (${subdir}_${target} ${target}.c)
		set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
		target_link_libraries (${subdir}_${target}
			${X11_GLU_LIB}
			${X11_GL_LIB}
			${X11_Xext_LIB}
			${X11_X11_LIB}
		)
		install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
	endforeach (target)
endif (X11_FOUND)
