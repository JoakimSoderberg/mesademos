include_directories(${EGL_INCLUDE_DIR}
  ../eglut
  ../../util
  )

add_executable(eglinfo eglinfo.c)
target_link_libraries(eglinfo ${EGL_egl_LIBRARY})

add_executable(eglgears_screen eglgears.c)
target_link_libraries(eglgears_screen ${EGL_egl_LIBRARY} eglut_screen)

if(X11_FOUND)
  add_executable(eglgears_x11 eglgears.c)
  target_link_libraries(eglgears_x11 ${EGL_egl_LIBRARY} eglut_x11)
endif(X11_FOUND)

