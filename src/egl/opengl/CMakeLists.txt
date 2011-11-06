include_directories(${EGL_INCLUDE_DIR}
  ../eglut
  ../../util
  )

link_libraries (
	${EGL_egl_LIBRARY}
	${OPENGL_gl_LIBRARY}
)

add_executable(eglinfo eglinfo.c)

add_executable(eglgears_screen eglgears.c)
target_link_libraries(eglgears_screen eglut_screen)

if(X11_FOUND)
	add_executable(eglgears_x11 eglgears.c)
	target_link_libraries(eglgears_x11 eglut_x11)
endif(X11_FOUND)

