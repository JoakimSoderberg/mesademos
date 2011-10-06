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

set (subdir demos)

set (targets
	arbfplight
	arbfslight
	arbocclude
	arbocclude2
	bounce
	clearspd
	copypix
	cubemap
	cuberender
	dinoshade
	dissolve
	drawpix
	engine
	fbo_firecube
	fbotexture
	fire
	fogcoord
	fplight
	fslight
	gamma
	gearbox
	gears
	geartrain
	glinfo
	gloss
	gltestperf
	ipers
	isosurf
	lodbias
	morph3d
	multiarb
	normal
	paltex
	pixeltest
	pointblast
	projtex
	ray
	readpix
	reflect
	renormal
	shadowtex
	singlebuffer
	spectex
	spriteblast
	stex3d
	teapot
	terrain
	tessdemo
	texcyl
	texenv
	textures
	trispd
	tunnel2
	tunnel
	vao_demo
	winpos
)

foreach (target ${targets})
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)

add_executable (rain rain.cxx particles.cxx)

install (TARGETS rain DESTINATION demos)
