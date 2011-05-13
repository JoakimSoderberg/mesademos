#ifndef GLUT_WRAP_H
#define GLUT_WRAP_H

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#endif /* ! GLUT_WRAP_H */
