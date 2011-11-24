include_directories(
	${EGL_INCLUDE_DIRS}
	${GLESV1_INCLUDE_DIRS}
	${VG_INCLUDE_DIRS}
)

set (subdir egl/oes_vg)

set (targets
	tex2vgimage
	vgimage2tex
)

if (X11_FOUND)
	foreach (target ${targets})
		add_executable (${target} ${target}.c)
		target_link_libraries (${target} ${VG_LIBRARIES} ${GLESV1_LIBRARIES} ${EGL_LIBRARIES} ${X11_X11_LIB})
		install (TARGETS ${target} DESTINATION ${subdir})
	endforeach (target)
endif ()
