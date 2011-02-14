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

set (subdir tests)

set (targets
	afsmultiarb
	antialias
	# XXX: Requires Linux kernel headers???
	#api_speed 
	arbfpspec
	arbfptest1
	arbfptexture
	arbfptrig
	arbgpuprog
	arbnpot
	arbnpot-mipmap
	arbvptest1
	arbvptest3
	arbvptorus
	arbvpwarpmesh
	arraytexture
	blendxor
	blitfb
	bufferobj
	bug_3050
	bug_3101
	bug_3195
	bug_texstore_i8
	bumpmap
	calibrate_rast
	condrender
	copypixrate
	cva
	cva_huge
	cylwrap
	# XXX: Depends on Mesa GL header
	#debugger
	drawbuffers2
	drawbuffers
	exactrast
	ext422square
	fbotest1
	fbotest2
	fbotest3
	fillrate
	floattex
	fogcoord
	fptest1
	fptexture
	# XXX: Requires Mesa source
	#getprocaddress
	getteximage
	glutfx
	interleave
	invert
	lineclip
	manytex
	mapbufrange
	minmag
	mipgen
	mipmap_comp
	mipmap_comp_tests
	mipmap_limits
	mipmap_tunnel
	mipmap_view
	multipal
	multitexarray
	multiwindow
	no_s3tc
	occlude
	packedpixels
	pbo
	persp_hint
	prim
	prog_parameter
	quads
	random
	readrate
	rubberband
	scissor
	scissor-viewport
	seccolor
	shader_api
	stencilreaddraw
	stencilwrap
	step
	streaming_rect
	subtex
	subtexrate
	tex1d
	texcmp
	texcompress2
	texcompsub
	texdown
	texfilt
	texgenmix
	texline
	texobj
	texrect
	unfilledclip
	vparray
	vpeval
	vptest1
	vptest2
	vptest3
	vptorus
	vpwarpmesh
	yuvrect
	yuvsquare
	zbitmap
	zcomp
	zdrawpix
	zreaddraw
)

if (UNIX)
	set (targets ${targets} texleak)
endif (UNIX)


foreach (target ${targets})
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)

if (X11_FOUND)
	include_directories (
		${X11_INCLUDE_DIR}
	)

	link_libraries (
		${X11_X11_LIBRARY}
	)

	set (xtargets
		auxbuffer
		jkrahntest
		sharedtex
		texobjshare
	)

	foreach (xtarget ${xtargets})
		add_executable (${xtarget} ${xtarget}.c)
	endforeach (xtarget)

	install (TARGETS ${xtargets} DESTINATION ${subdir})
endif (X11_FOUND)
