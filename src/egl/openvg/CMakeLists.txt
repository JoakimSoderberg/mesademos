include_directories(
	${CMAKE_SOURCE_DIR}/src/egl/eglut
	${CMAKE_SOURCE_DIR}/src/util
	${EGL_INCLUDE_DIRS}
	${VG_INCLUDE_DIRS}
)

set (subdir egl/openvg)

if (X11_FOUND)
	foreach (target ${targets})
		add_executable (text text.c)
		target_link_libraries (text ${EGL_LIBRARIES} ${VG_LIBRARIES} ${X11_X11_LIB})
		install (TARGETS text DESTINATION ${subdir})
	endforeach (target)
endif ()


# Targets that can be built both for fullscreen EGL and X11

set (targets
	lion
	sp
)

foreach (target ${targets})
	set (sources ${target}.c)

	if (${target} STREQUAL lion)
		set (sources ${sources} lion-render.c)
	endif ()

	add_executable (${target}_screen ${sources})
	target_link_libraries (${target}_screen eglut_screen ${VG_LIBRARIES})
	install (TARGETS ${target}_screen DESTINATION ${subdir})

	if (X11_FOUND)
		add_executable (${target}_x11 ${sources})
		target_link_libraries (${target}_x11 eglut_x11 ${VG_LIBRARIES})
		install (TARGETS ${target}_x11 DESTINATION ${subdir})
	endif ()
endforeach (target)

add_subdirectory (trivial)
