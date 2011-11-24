include_directories (
	${EGL_INCLUDE_DIRS}
)

if (X11_FOUND)
	add_library (eglut_x11 eglut.h eglut.c eglutint.h eglut_x11.c)
	target_link_libraries (eglut_x11 ${EGL_LIBRARIES} ${X11_X11_LIB})
	if (BUILD_SHARED_LIBS)
		install (TARGETS eglut_x11 DESTINATION ${LIBDIR})
	endif (BUILD_SHARED_LIBS)
endif (X11_FOUND)

add_library (eglut_screen eglut.h eglut.c eglutint.h eglut_screen.c)
target_link_libraries (eglut_screen ${EGL_LIBRARIES})
if (BUILD_SHARED_LIBS)
	install (TARGETS eglut_screen DESTINATION ${LIBDIR})
endif (BUILD_SHARED_LIBS)
