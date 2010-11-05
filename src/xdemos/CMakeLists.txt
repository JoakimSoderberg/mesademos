include_directories (
	${mesademos_SOURCE_DIR}/src/util
	${OPENGL_INCLUDE_PATH}
	${X11_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_directories (
	${mesademos_SOURCE_DIR}/src/util
)

link_libraries (
	util
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${X11_X11_LIBRARY}
	${GLEW_glew_LIBRARY}
)

add_library (pbutil pbutil.c)

set (targets
	glsync
	glthreads
	glxcontexts
	glxdemo
	glxgears
	glxgears_fbconfig
	glxgears_pixmap
	glxheads
	glxinfo
	glxpbdemo
	glxpixmap
	glxsnoop
	glxswapcontrol
	manywin
	msctest
	multictx
	offset
	omlsync
	opencloseopen
	overlay
	pbdemo
	pbinfo
	shape
	sharedtex
	sharedtex_mt
	texture_from_pixmap
	wincopy
	# XXX: Requires xmesa.h
	#xdemo
	xfont
	yuvrect_client
)

foreach (target ${targets})
	add_executable (${target} ${target}.c)
endforeach (target)

target_link_libraries (glthreads pthread)
target_link_libraries (glxgears_fbconfig pbutil)
target_link_libraries (pbdemo pbutil)
target_link_libraries (pbinfo pbutil)
target_link_libraries (sharedtex_mt pthread)

add_executable (corender corender.c ipc.c) 
add_executable (xrotfontdemo xrotfontdemo.c xuserotfont.c)

install (TARGETS ${targets} corender xrotfontdemo DESTINATION xdemos)
