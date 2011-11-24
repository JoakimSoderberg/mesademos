
add_subdirectory(eglut)

add_subdirectory(opengl)

if (GLESV1_FOUND)
	add_subdirectory(opengles1)
endif ()

if (GLESV2_FOUND)
	add_subdirectory(opengles2)
endif ()

if (VG_FOUND)
	add_subdirectory(openvg)
endif ()

if (GLESV1_FOUND AND VG_FOUND)
	add_subdirectory(oes_vg)
endif ()
