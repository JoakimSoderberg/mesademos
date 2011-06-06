cmake_minimum_required (VERSION 2.8)

project (mesademos)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
find_package (X11)

find_library(EGL_egl_LIBRARY EGL /usr/lib)

find_library (GLEW_glew_LIBRARY GLEW
	/usr/lib
)

find_path (GLEW_INCLUDE_DIR GL/glew.h
      /usr/include/GL
)

if (UNIX)
	link_libraries(m)
endif (UNIX)

# On Mac OS X, GLX is provided as a separate OpenGL implementation, different
# from the standard OpenGL framework which provides support for GLUT and native
# Mac OS X applications.
if (X11_FOUND)
	if (APPLE)
		find_path (X11_GL_INCLUDE_PATH GL/glx.h ${X11_INC_SEARCH_PATH})
		if (NOT X11_GL_INCLUDE_PATH)
			message (WARNING "Could not find GL/glx.h")
			set (X11_FOUND FALSE)
		endif (NOT X11_GL_INCLUDE_PATH)
		set (X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${X11_GL_INCLUDE_PATH})

		find_library (X11_GL_LIB GL ${X11_LIB_SEARCH_PATH})
		if (NOT X11_GL_LIB)
			message (WARNING "Could not find libGL.dylib")
			set (X11_FOUND FALSE)
		endif (NOT X11_GL_LIB)

		find_library (X11_GLU_LIB GLU ${X11_LIB_SEARCH_PATH})
		if (NOT X11_GLU_LIB)
			message (WARNING "Could not find libGLU.dylib")
			set (X11_FOUND FALSE)
		endif (NOT X11_GLU_LIB)
	else ()
		set (X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
		set (X11_GL_LIB ${OPENGL_gl_LIBRARY})
		set (X11_GLU_LIB ${OPENGL_glu_LIBRARY})
	endif ()
endif (X11_FOUND)

if (WIN32)
	# Nobody likes to include windows.h:
	# - Microsoft's GL/gl.h header depends on windows.h but doesn't include it;
	# - GLEW temporarily defines the necessary defines but undefines them later
	# - certain GLUT distributions don't include it;
	# - most of our programs are meant to be portable so don't include it.
	#
	# We could try to replicate the windows.h definitions required by
	# GL/gl.h, but the build time savings don't compensate the constant
	# headaches that brings, so instead we force windows.h to be included
	# on every file.
	if (MSVC)
		add_definitions (-FIwindows.h)
	else (MSVC)
		add_definitions (--include windows.h)
	endif (MSVC)

	# Don't define min/max macros
	add_definitions (-DNOMINMAX)

	# MSVC & MinGW only define & use APIENTRY
	add_definitions (-DGLAPIENTRY=__stdcall)

	link_libraries (winmm)
endif (WIN32)

if (MSVC)
	# Enable math constants defines
	add_definitions (-D_USE_MATH_DEFINES)

	# Silence several MSVC pedantic warnings
	add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
	add_definitions (-D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
	add_definitions (-wd4244) # conversion' conversion from 'type1' to 'type2', possible loss of data
endif (MSVC)

add_definitions(-DDEMOS_DATA_DIR=\"../data/\")

add_subdirectory (src)


install (FILES index.html DESTINATION doc)

set (CPACK_PACKAGE_NAME "mesa-demos")
set (CPACK_PACKAGE_VERSION_MAJOR "8")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "1")

if (WIN32)
    set (CPACK_GENERATOR "ZIP")
elseif (APPLE)
    set (CPACK_GENERATOR "DragNDrop")
    set (CPACK_DMG_FORMAT "UDBZ")
else ()
    set (CPACK_GENERATOR "TBZ2")
endif ()

include(CPack)
