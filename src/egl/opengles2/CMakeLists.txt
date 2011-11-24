include_directories(
	${CMAKE_SOURCE_DIR}/src/egl/eglut
	${CMAKE_SOURCE_DIR}/src/util
	${EGL_INCLUDE_DIRS}
	${GLESV2_INCLUDE_DIRS}
)

set (subdir egl/opengles2)

set (targets
	es2_info
	es2tri
)

if (X11_FOUND)
	foreach (target ${targets})
		add_executable (${target} ${target}.c)
		target_link_libraries (${target} ${EGL_LIBRARIES} ${GLESV2_LIBRARIES} ${X11_X11_LIB})
		install (TARGETS ${target} DESTINATION ${subdir})
	endforeach (target)
endif ()


# Targets that can be built both for fullscreen EGL and X11

set (targets
	es2gears
)

foreach (target ${targets})
	add_executable (${target}_screen ${target}.c)
	target_link_libraries (${target}_screen eglut_screen ${GLESV2_LIBRARIES})
	install (TARGETS ${target}_screen DESTINATION ${subdir})

	if (X11_FOUND)
		add_executable (${target}_x11 ${target}.c)
		target_link_libraries (${target}_x11 eglut_x11 ${GLESV2_LIBRARIES})
		install (TARGETS ${target}_x11 DESTINATION ${subdir})
	endif ()
endforeach (target)
