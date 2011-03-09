add_subdirectory (util)

add_subdirectory (demos)
add_subdirectory (tests)
add_subdirectory (trivial)
add_subdirectory (redbook)
add_subdirectory (samples)
add_subdirectory (perf)
add_subdirectory (objviewer)

add_subdirectory (glsl)
add_subdirectory (fp)
add_subdirectory (fpglsl)
add_subdirectory (vp)
add_subdirectory (vpglsl)
add_subdirectory (gs)

if(EGL_egl_LIBRARY)
  add_subdirectory(egl)
endif(EGL_egl_LIBRARY)

if (X11_FOUND)
	add_subdirectory (xdemos)
endif (X11_FOUND)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_subdirectory (wgl)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

add_subdirectory (data)
