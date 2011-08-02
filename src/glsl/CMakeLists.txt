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

set (subdir glsl)
set (targets
	array
	bezier
	bitmap
	brick
	bump
	convolutions
	deriv
	fragcoord
	fsraytrace
	geom-sprites
	geom-stipple-lines
	geom-wide-lines
	identity
	linktest
	mandelbrot
	multinoise
	multitex
	noise2
	noise
	pointcoord
	points
	samplers
	shadow_sampler
	shtest
	simplex-noise
	skinning
	texaaline
	texdemo1
	toyball
	trirast
	twoside
	vert-or-frag-only
	vert-tex
	vsraytrace
)

foreach (target ${targets})
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)

file (GLOB data *.frag *.vert *.shtest)

install (FILES ${data} DESTINATION ${subdir})
