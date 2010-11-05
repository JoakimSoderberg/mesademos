include_directories(
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
	arbfplight
	arbfslight
	arbocclude
	arbocclude2
	bounce
	clearspd
	copypix
	cubemap
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
	paltex
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
	add_executable (${target} ${target}.c)
endforeach (target)

install (TARGETS ${targets} DESTINATION demos)

install (FILES geartrain.dat isosurf.dat terrain.dat DESTINATION demos)
