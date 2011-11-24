include_directories(
	${CMAKE_SOURCE_DIR}/src/egl/eglut
	${CMAKE_SOURCE_DIR}/src/util
	${EGL_INCLUDE_DIRS}
	${GLESV1_INCLUDE_DIRS}
)

set (subdir egl/opengles1)

set (targets
	bindtex
	es1_info
	msaa
	pbuffer
	render_tex
	texture_from_pixmap
	two_win
)

if (X11_FOUND)
	foreach (target ${targets})
		add_executable (${target} ${target}.c)
		target_link_libraries (${target} ${EGL_LIBRARIES} ${GLESV1_LIBRARIES} ${X11_X11_LIB})
		install (TARGETS ${target} DESTINATION ${subdir})
	endforeach (target)
endif ()


# Targets that can be built both for fullscreen EGL and X11


set (targets
	drawtex
	gears
	torus
	tri
)

foreach (target ${targets})
	add_executable (${target}_screen ${target}.c)
	target_link_libraries (${target}_screen eglut_screen ${GLESV1_LIBRARIES})
	install (TARGETS ${target}_screen DESTINATION ${subdir})

	if (X11_FOUND)
		add_executable (${target}_x11 ${target}.c)
		target_link_libraries (${target}_x11 eglut_x11 ${GLESV1_LIBRARIES})
		install (TARGETS ${target}_x11 DESTINATION ${subdir})
	endif ()
endforeach (target)

if (X11_FOUND)
	# intentionally not link to GLESV1_LIBS
	add_executable (clear clear.c)
	target_link_libraries (clear eglut_x11)
	install (TARGETS clear DESTINATION ${subdir})
endif ()
	
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_executable (eglfbdev eglfbdev.c)
	target_link_libraries (eglfbdev ${EGL_LIBRARIES} ${GLESV1_LIBRARIES})
	install (TARGETS eglfbdev DESTINATION ${subdir})
endif ()
