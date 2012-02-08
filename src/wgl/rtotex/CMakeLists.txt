include_directories (
	${OPENGL_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_libraries (
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLUT_glut_LIBRARY}
	${GLEW_glew_LIBRARY}
)

add_executable (rtotex WIN32
	fps_counter.cpp
	image.cpp
	interactor.cpp
	log.cpp
	main.cpp
	pbuffer.cpp
	timer.cpp
	window.cpp
	maths/color.cpp
	maths/matrix4x4.cpp
	maths/plane.cpp
	maths/vector2d.cpp
	maths/vector3d.cpp
	maths/vector4d.cpp
)

install (TARGETS rtotex DESTINATION wgl/rtotex)

install (FILES decal.bmp DESTINATION wgl/rtotex)

