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

set (subdir redbook)

set (targets
	aaindex
	aapoly
	aargb
	accanti
	accpersp
	alpha3D
	alpha
	anti
	bezcurve
	bezmesh
	checker
	clip
	colormat
	combiner
	convolution
	cube
	cubemap
	depthcue
	dof
	double
	drawf
	feedback
	fog
	fogcoord
	fogindex
	font
	hello
	histogram
	image
	light
	lines
	list
	material
	minmax
	mipmap
	model
	movelight
	multisamp
	multitex
	mvarray
	nurbs
	pickdepth
	picksquare
	plane
	planet
	pointp
	polyoff
	polys
	quadric
	robot
	sccolorlight
	scenebamb
	scene
	sceneflat
	select
	shadowmap
	smooth
	stencil
	stroke
	surface
	surfpoints
	teaambient
	teapots
	tess
	tesswind
	texbind
	texgen
	texprox
	texsub
	texture3d
	texturesurf
	torus
	trim
	unproject
	varray
	wrap
)

foreach (target ${targets})
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)
