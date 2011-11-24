include_directories(
	${EGL_INCLUDE_DIRS}
	${VG_INCLUDE_DIRS}
)

set (subdir egl/openvg/trivial)

add_library (common STATIC eglcommon)

set (targets
	arc
	cap
	blend
	clear
	color_transform
	coord
	dash
	ellipse
	filter
	gradorigin
	image
	layer
	lineto
	lingrad
	lookup
	mask4
	mask
	mask_render
	paint
	path3
	radialgrad
	readpixels
	roundedrect
	star-nonzero
	star-oddeven
	stroke2
	stroke
# VGU is broken on current mesa master
#	vguarc
)

foreach (target ${targets})

	if (X11_FOUND)
		add_executable (openvg_${target} ${target}.c)
		target_link_libraries (openvg_${target} common ${VG_LIBRARIES} ${EGL_LIBRARIES} ${X11_X11_LIB})
		set_target_properties (openvg_${target} PROPERTIES OUTPUT_NAME ${target})
		install (TARGETS openvg_${target} DESTINATION ${subdir})
	endif ()
endforeach (target)
