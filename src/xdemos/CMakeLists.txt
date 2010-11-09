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

set (subdir xdemos)

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
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)

target_link_libraries (${subdir}_glthreads pthread)
target_link_libraries (${subdir}_glxgears_fbconfig pbutil)
target_link_libraries (${subdir}_pbdemo pbutil)
target_link_libraries (${subdir}_pbinfo pbutil)
target_link_libraries (${subdir}_sharedtex_mt pthread)

add_executable (corender corender.c ipc.c) 
add_executable (xrotfontdemo xrotfontdemo.c xuserotfont.c)

install (TARGETS corender xrotfontdemo DESTINATION demos)
