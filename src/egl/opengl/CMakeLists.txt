include_directories(
	${CMAKE_SOURCE_DIR}/src/egl/eglut
	${CMAKE_SOURCE_DIR}/src/util
	${EGL_INCLUDE_DIR}
	${OPENGL_INCLUDE_PATH}
)

link_libraries (
	${EGL_egl_LIBRARY}
	${OPENGL_gl_LIBRARY}
)

set (subdir egl)

set (targets
	demo1
	demo2
	demo3
)

foreach (target ${targets})
	add_executable (${subdir}_${target} ${target}.c)
	set_target_properties (${subdir}_${target} PROPERTIES OUTPUT_NAME ${target})
	install (TARGETS ${subdir}_${target} DESTINATION ${subdir})
endforeach (target)


# Targets that can be built both for fullscreen EGL and X11

set (targets
	egltri
	eglgears
)

foreach (target ${targets})
	add_executable (${target}_screen ${target}.c)
	target_link_libraries (${target}_screen eglut_screen)
	install (TARGETS ${target}_screen DESTINATION ${subdir})

	if (X11_FOUND)
		add_executable (${target}_x11 ${target}.c)
		target_link_libraries (${target}_x11 eglut_x11)
		install (TARGETS ${target}_x11 DESTINATION ${subdir})
	endif ()
endforeach (target)


if (X11_FOUND)
	add_executable (xeglgears xeglgears.c)
	target_link_libraries (xeglgears ${X11_X11_LIB})

	add_executable (xeglthreads xeglthreads.c)
	target_link_libraries (xeglthreads ${X11_X11_LIB})
endif (X11_FOUND)

