include_directories (
	${OPENGL_INCLUDE_PATH}
)

link_libraries (
	${OPENGL_gl_LIBRARY}
)

add_executable (wglthreads wglthreads/wglthreads.c)
add_executable (sharedtex_mt sharedtex_mt/sharedtex_mt.c)
add_executable (wglinfo wglinfo.c)

install (TARGETS wglthreads sharedtex_mt wglthreads DESTINATION wgl)
