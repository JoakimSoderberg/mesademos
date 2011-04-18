/**
 * Test using a geometry and fragment shaders to implement stippled lines.
 *
 * Brian Paul
 * April 2011
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaderutil.h"

static GLint WinWidth = 500, WinHeight = 500;
static GLint Win = 0;
static GLuint VertShader, GeomShader, FragShader, Program;
static GLboolean Anim = GL_TRUE;
static GLboolean UseGeomShader = GL_TRUE;
static GLfloat Xrot = 0, Yrot = 0;
static int uViewportSize = -1, uStippleTex = -1, uStippleFactor;
static const int NumPoints = 50;
static float Points[100][3], Colors[100][3];

static const GLushort StipplePattern = 0x10ff;
static GLuint StippleFactor = 2;


static void
CheckError(int line)
{
   GLenum err = glGetError();
   if (err) {
      printf("GL Error %s (0x%x) at line %d\n",
             gluErrorString(err), (int) err, line);
   }
}


static GLuint
MakeStippleTexture(GLushort pattern)
{
   GLuint tex, i;
   GLubyte image[16];

   for (i = 0; i < 16; i++) {
      image[i] = (pattern & (1 << i)) ? 0xff : 0;
   }

   glGenTextures(1, &tex);
   glBindTexture(GL_TEXTURE_1D, tex);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE, 16, 0,
                GL_LUMINANCE, GL_UNSIGNED_BYTE, image);

   return tex;
}


static void
Redisplay(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();
   glRotatef(Xrot, 1, 0, 0);
   glRotatef(Yrot, 0, 0, 1);

   if (UseGeomShader) {
      glUseProgram(Program);
      glDisable(GL_LINE_STIPPLE);
   }
   else {
      glUseProgram(0);
      glEnable(GL_LINE_STIPPLE);
   }

   glBegin(GL_LINES);
   for (i = 0; i < NumPoints; i++) {
      glColor3fv(Colors[i]);
      glVertex3fv(Points[i]);
   }
   glEnd();

   glPopMatrix();

   glutSwapBuffers();
}


static void
Idle(void)
{
   int curTime = glutGet(GLUT_ELAPSED_TIME);
   Xrot = curTime * 0.02;
   Yrot = curTime * 0.05;
   glutPostRedisplay();
}


static void
Reshape(int width, int height)
{
   float ar = (float) width / height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
#if 1
   glFrustum(-ar, ar, -1, 1, 3, 25);
#else
   glOrtho(-3.0*ar, 3.0*ar, -3.0, 3.0, 3, 25);
#endif
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0, 0, -10);

   {
      GLfloat viewport[4];
      glGetFloatv(GL_VIEWPORT, viewport);
      glUniform2f(uViewportSize, viewport[2], viewport[3]);
   }
}


static void
CleanUp(void)
{
   glDeleteShader(FragShader);
   glDeleteShader(VertShader);
   glDeleteShader(GeomShader);
   glDeleteProgram(Program);
   glutDestroyWindow(Win);
}


static void
Key(unsigned char key, int x, int y)
{
  (void) x;
  (void) y;

   switch(key) {
   case ' ':
   case 'a':
      Anim = !Anim;
      if (Anim) {
         glutIdleFunc(Idle);
      }
      else
         glutIdleFunc(NULL);
      break;
   case 'g':
      UseGeomShader = !UseGeomShader;
      printf("Use geometry shader? %d\n", UseGeomShader);
      break;
   case 'x':
      Xrot ++;
      break;
   case 27:
      CleanUp();
      exit(0);
      break;
   }
   glutPostRedisplay();
}


static void
MakePoints(void)
{
   int i;
   for (i = 0; i < NumPoints; i++) {
      Colors[i][0] = (rand() % 1000) / 1000.0;
      Colors[i][1] = (rand() % 1000) / 1000.0;
      Colors[i][2] = (rand() % 1000) / 1000.0;
      Points[i][0] = ((rand() % 2000) - 1000.0) / 500.0;
      Points[i][1] = ((rand() % 2000) - 1000.0) / 500.0;
      Points[i][2] = ((rand() % 2000) - 1000.0) / 500.0;
   }
}


static void
Init(void)
{
   GLuint tex;

   static const char *fragShaderText =
      "uniform sampler1D StippleTex; \n"
      "varying float stippleCoord; \n"
      "void main() \n"
      "{ \n"
      "   // sample the stipple pattern and discard if value is zero \n"
      "   // TODO: we should really undo the perspective interpolation here \n"
      "   // so that it's linear. \n"
      "   vec4 stip = texture1D(StippleTex, stippleCoord); \n"
      "   if (stip.x == 0.0) \n"
      "      discard; \n"
      "   gl_FragColor = gl_Color; \n"
      "} \n";
   static const char *vertShaderText =
      "void main() \n"
      "{ \n"
      "   gl_FrontColor = gl_Color; \n"
      "   gl_Position = ftransform(); \n"
      "} \n";
   static const char *geomShaderText =
      "#version 120 \n"
      "#extension GL_ARB_geometry_shader4: enable \n"
      "uniform vec2 ViewportSize; \n"
      "uniform float StippleFactor; \n"
      "varying float stippleCoord; \n"
      "void main() \n"
      "{ \n"
      "   vec4 pos0 = gl_PositionIn[0]; \n"
      "   vec4 pos1 = gl_PositionIn[1]; \n"
      "   // Convert eye coords to window coords \n"
      "   // Note: we're off by a factor of two here, make up for that below \n"
      "   vec2 p0 = pos0.xy / pos0.w * ViewportSize; \n"
      "   vec2 p1 = pos1.xy / pos1.w * ViewportSize; \n"
      "   float len = length(p0.xy - p1.xy); \n"
      "   len /= StippleFactor; \n"
      "   // Emit first vertex \n"
      "   gl_FrontColor = gl_FrontColorIn[0]; \n"
      "   gl_Position = pos0; \n"
      "   stippleCoord = 0.0; \n"
      "   EmitVertex(); \n"
      "   // Emit second vertex \n"
      "   gl_FrontColor = gl_FrontColorIn[1]; \n"
      "   gl_Position = pos1; \n"
      "   stippleCoord = len / 32.0; // Note: not 16, see above \n"
      "   EmitVertex(); \n"
      "} \n";

   if (!ShadersSupported())
      exit(1);

   if (!glutExtensionSupported("GL_ARB_geometry_shader4")) {
      fprintf(stderr, "Sorry, GL_ARB_geometry_shader4 is not supported.\n");
      exit(1);
   }

   VertShader = CompileShaderText(GL_VERTEX_SHADER, vertShaderText);
   FragShader = CompileShaderText(GL_FRAGMENT_SHADER, fragShaderText);
   GeomShader = CompileShaderText(GL_GEOMETRY_SHADER_ARB, geomShaderText);
   assert(GeomShader);

   Program = LinkShaders3(VertShader, GeomShader, FragShader);
   assert(Program);
   CheckError(__LINE__);

   /*
    * The geometry shader accepts lines and produces lines.
    */
   glProgramParameteriARB(Program, GL_GEOMETRY_INPUT_TYPE_ARB,
                          GL_LINES);
   glProgramParameteriARB(Program, GL_GEOMETRY_OUTPUT_TYPE_ARB,
                          GL_LINE_STRIP);
   glProgramParameteriARB(Program, GL_GEOMETRY_VERTICES_OUT_ARB, 4);
   CheckError(__LINE__);

   glLinkProgramARB(Program);

   /* check link */
   {
      GLint stat;
      GetProgramiv(Program, GL_LINK_STATUS, &stat);
      if (!stat) {
         GLchar log[1000];
         GLsizei len;
         GetProgramInfoLog(Program, 1000, &len, log);
         fprintf(stderr, "Shader link error:\n%s\n", log);
      }
   }

   glUseProgram(Program);

   uViewportSize = glGetUniformLocation(Program, "ViewportSize");
   uStippleTex = glGetUniformLocation(Program, "StippleTex");
   uStippleFactor = glGetUniformLocation(Program, "StippleFactor");

   glUniform1i(uStippleTex, 0); /* texture unit 0 */
   glUniform1f(uStippleFactor, StippleFactor);

   glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

   printf("GL_RENDERER = %s\n",(const char *) glGetString(GL_RENDERER));

   assert(glIsProgram(Program));
   assert(glIsShader(FragShader));
   assert(glIsShader(VertShader));
   assert(glIsShader(GeomShader));


   glLineStipple(StippleFactor, StipplePattern);
   tex = MakeStippleTexture(StipplePattern);

   MakePoints();
}


int
main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(WinWidth, WinHeight);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   Win = glutCreateWindow(argv[0]);
   glewInit();
   glutReshapeFunc(Reshape);
   glutKeyboardFunc(Key);
   glutDisplayFunc(Redisplay);
   if (Anim)
      glutIdleFunc(Idle);

   Init();
   glutMainLoop();
   return 0;
}
