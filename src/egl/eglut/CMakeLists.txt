if(X11_FOUND)
  add_library(eglut_x11 eglut.h eglut.c eglutint.h eglut_x11.c)
  target_link_libraries(eglut_x11 ${EGL_egl_LIBRARY})
endif(X11_FOUND)

add_library(eglut_screen eglut.h eglut.c eglutint.h eglut_screen.c)
target_link_libraries(eglut_screen ${EGL_egl_LIBRARY})
