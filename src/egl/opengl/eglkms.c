#include <stdio.h>
#include <stdlib.h>

#define EGL_EGLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES

#include <gbm.h>
#include "gl_wrap.h"
#include <GL/glext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <drm.h>
#include <xf86drmMode.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#ifdef GL_OES_EGL_image
static PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC glEGLImageTargetRenderbufferStorageOES_func;
#endif

struct kms {
   drmModeConnector *connector;
   drmModeEncoder *encoder;
   drmModeModeInfo mode;
   uint32_t fb_id;
};

static EGLBoolean
setup_kms(int fd, struct kms *kms)
{
   drmModeRes *resources;
   drmModeConnector *connector;
   drmModeEncoder *encoder;
   int i;

   resources = drmModeGetResources(fd);
   if (!resources) {
      fprintf(stderr, "drmModeGetResources failed\n");
      return EGL_FALSE;
   }

   for (i = 0; i < resources->count_connectors; i++) {
      connector = drmModeGetConnector(fd, resources->connectors[i]);
      if (connector == NULL)
	 continue;

      if (connector->connection == DRM_MODE_CONNECTED &&
	  connector->count_modes > 0)
	 break;

      drmModeFreeConnector(connector);
   }

   if (i == resources->count_connectors) {
      fprintf(stderr, "No currently active connector found.\n");
      return EGL_FALSE;
   }

   for (i = 0; i < resources->count_encoders; i++) {
      encoder = drmModeGetEncoder(fd, resources->encoders[i]);

      if (encoder == NULL)
	 continue;

      if (encoder->encoder_id == connector->encoder_id)
	 break;

      drmModeFreeEncoder(encoder);
   }

   kms->connector = connector;
   kms->encoder = encoder;
   kms->mode = connector->modes[0];

   return EGL_TRUE;
}

static void
render_stuff(int width, int height)
{
   GLfloat view_rotx = 0.0, view_roty = 0.0, view_rotz = 0.0;
   static const GLfloat verts[3][2] = {
      { -1, -1 },
      {  1, -1 },
      {  0,  1 }
   };
   static const GLfloat colors[3][3] = {
      { 1, 0, 0 },
      { 0, 1, 0 },
      { 0, 0, 1 }
   };
   GLfloat ar = (GLfloat) width / (GLfloat) height;

   glViewport(0, 0, (GLint) width, (GLint) height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-ar, ar, -1, 1, 5.0, 60.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -10.0);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.4, 0.4, 0.4, 0.0);

   glPushMatrix();
   glRotatef(view_rotx, 1, 0, 0);
   glRotatef(view_roty, 0, 1, 0);
   glRotatef(view_rotz, 0, 0, 1);

   glVertexPointer(2, GL_FLOAT, 0, verts);
   glColorPointer(3, GL_FLOAT, 0, colors);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   glDrawArrays(GL_TRIANGLES, 0, 3);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

   glPopMatrix();

   glFinish();
}

static const char device_name[] = "/dev/dri/card0";

int main(int argc, char *argv[])
{
   EGLDisplay dpy;
   EGLContext ctx;
   EGLImageKHR image;
   EGLint major, minor;
   const char *ver, *extensions;
   GLuint fb, color_rb, depth_rb;
   uint32_t handle, stride;
   struct kms kms;
   int ret, fd;
   struct gbm_device *gbm;
   struct gbm_bo *bo;
   drmModeCrtcPtr saved_crtc;

   fd = open(device_name, O_RDWR);
   if (fd < 0) {
      /* Probably permissions error */
      fprintf(stderr, "couldn't open %s, skipping\n", device_name);
      return -1;
   }

   gbm = gbm_create_device(fd);
   if (gbm == NULL) {
      fprintf(stderr, "couldn't create gbm device\n");
      ret = -1;
      goto close_fd;
   }

   dpy = eglGetDisplay(gbm);
   if (dpy == EGL_NO_DISPLAY) {
      fprintf(stderr, "eglGetDisplay() failed\n");
      ret = -1;
      goto destroy_gbm_device;
   }
	
   if (!eglInitialize(dpy, &major, &minor)) {
      printf("eglInitialize() failed\n");
      ret = -1;
      goto egl_terminate;
   }

   ver = eglQueryString(dpy, EGL_VERSION);
   printf("EGL_VERSION = %s\n", ver);

   extensions = eglQueryString(dpy, EGL_EXTENSIONS);
   printf("EGL_EXTENSIONS: %s\n", extensions);

   if (!strstr(extensions, "EGL_KHR_surfaceless_opengl")) {
      printf("No support for EGL_KHR_surfaceless_opengl\n");
      ret = -1;
      goto egl_terminate;
   }

   if (!setup_kms(fd, &kms)) {
      ret = -1;
      goto egl_terminate;
   }

   eglBindAPI(EGL_OPENGL_API);
   ctx = eglCreateContext(dpy, NULL, EGL_NO_CONTEXT, NULL);
   if (ctx == NULL) {
      fprintf(stderr, "failed to create context\n");
      ret = -1;
      goto egl_terminate;
   }

   if (!eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, ctx)) {
      fprintf(stderr, "failed to make context current\n");
      ret = -1;
      goto destroy_context;
   }

#ifdef GL_OES_EGL_image
   glEGLImageTargetRenderbufferStorageOES_func =
      (PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC)
      eglGetProcAddress("glEGLImageTargetRenderbufferStorageOES");
#else
   fprintf(stderr, "GL_OES_EGL_image not supported at compile time\n");
#endif

   glGenFramebuffers(1, &fb);
   glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);

   bo = gbm_bo_create(gbm, kms.mode.hdisplay, kms.mode.vdisplay,
		      GBM_BO_FORMAT_XRGB8888,
		      GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
   if (bo == NULL) {
      fprintf(stderr, "failed to create gbm bo\n");
      ret = -1;
      goto unmake_current;
   }
   handle = gbm_bo_get_handle(bo).u32;
   stride = gbm_bo_get_pitch(bo);

   image = eglCreateImageKHR(dpy, NULL, EGL_NATIVE_PIXMAP_KHR, bo, NULL);
   if (image == EGL_NO_IMAGE_KHR) {
      fprintf(stderr, "failed to create egl image\n");
      ret = -1;
      goto destroy_gbm_bo;
   }

   glGenRenderbuffers(1, &color_rb);
   glBindRenderbuffer(GL_RENDERBUFFER_EXT, color_rb);
#ifdef GL_OES_EGL_image
   glEGLImageTargetRenderbufferStorageOES(GL_RENDERBUFFER, image);
#else
   fprintf(stderr, "GL_OES_EGL_image was not found at compile time\n");
#endif
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
				GL_COLOR_ATTACHMENT0_EXT,
				GL_RENDERBUFFER_EXT,
				color_rb);
   
   glGenRenderbuffers(1, &depth_rb);
   glBindRenderbuffer(GL_RENDERBUFFER_EXT, depth_rb);
   glRenderbufferStorage(GL_RENDERBUFFER_EXT,
			 GL_DEPTH_COMPONENT,
			 kms.mode.hdisplay, kms.mode.vdisplay);
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
				GL_DEPTH_ATTACHMENT_EXT,
				GL_RENDERBUFFER_EXT,
				depth_rb);

   if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) !=
       GL_FRAMEBUFFER_COMPLETE) {
      fprintf(stderr, "framebuffer not complete\n");
      ret = 1;
      goto rm_rb;
   }

   render_stuff(kms.mode.hdisplay, kms.mode.vdisplay);

   printf("handle=%d, stride=%d\n", handle, stride);

   ret = drmModeAddFB(fd,
		      kms.mode.hdisplay, kms.mode.vdisplay,
		      32, 32, stride, handle, &kms.fb_id);
   if (ret) {
      fprintf(stderr, "failed to create fb\n");
      goto rm_rb;
   }

   saved_crtc = drmModeGetCrtc(fd, kms.encoder->crtc_id);
   if (saved_crtc == NULL)
      goto rm_fb;

   ret = drmModeSetCrtc(fd, kms.encoder->crtc_id, kms.fb_id, 0, 0,
			&kms.connector->connector_id, 1, &kms.mode);
   if (ret) {
      fprintf(stderr, "failed to set mode: %m\n");
      goto free_saved_crtc;
   }

   getchar();

   ret = drmModeSetCrtc(fd, saved_crtc->crtc_id, saved_crtc->buffer_id,
                        saved_crtc->x, saved_crtc->y,
                        &kms.connector->connector_id, 1, &saved_crtc->mode);
   if (ret) {
      fprintf(stderr, "failed to restore crtc: %m\n");
   }

free_saved_crtc:
   drmModeFreeCrtc(saved_crtc);
rm_rb:
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
				GL_COLOR_ATTACHMENT0_EXT,
				GL_RENDERBUFFER_EXT, 0);
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
				GL_DEPTH_ATTACHMENT_EXT,
				GL_RENDERBUFFER_EXT, 0);
   glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
   glDeleteRenderbuffers(1, &color_rb);
   glDeleteRenderbuffers(1, &depth_rb);
rm_fb:
   drmModeRmFB(fd, kms.fb_id);
   eglDestroyImageKHR(dpy, image);
destroy_gbm_bo:
   gbm_bo_destroy(bo);
unmake_current:
   eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
destroy_context:
   eglDestroyContext(dpy, ctx);
egl_terminate:
   eglTerminate(dpy);
destroy_gbm_device:
   gbm_device_destroy(gbm);
close_fd:
   close(fd);

   return ret;
}
