#include "eglcommon.h"

#include <VG/openvg.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

VGPath rect;

static void
init(void)
{
   VGPaint paint;

   VGubyte cmd[] = {
      VG_MOVE_TO_ABS,
      VG_LINE_TO_ABS,
      VG_LINE_TO_ABS,
      VG_LINE_TO_ABS,
      VG_CLOSE_PATH
   };
   VGfloat val[] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
   };

   rect = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                       VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0,
                       VG_PATH_CAPABILITY_ALL);
   vgAppendPathData(rect, sizeof(cmd), cmd, val);

   paint = vgCreatePaint();
   /* alpha = 0.8 */
   vgSetColor(paint, 0xff0000cc);
   vgSetPaint(paint, VG_FILL_PATH);

   vgSeti(VG_MASKING, VG_TRUE);
}

/* new window size or exposure */
static void
reshape(int w, int h)
{
   VGfloat coverage[4] = { 0.0f, 0.0f, 0.0f, 0.4f };
   VGImage img;

   img = vgCreateImage(VG_A_8, w, h / 2, VG_IMAGE_QUALITY_NONANTIALIASED);
   vgSetfv(VG_CLEAR_COLOR, 4, coverage);
   vgClearImage(img, 0, 0, w, h / 2);

   vgMask(img, VG_SET_MASK, 0, (h + 1) / 2, w, h / 2);
}

static void
rectangle(VGint x, VGint y, VGint width, VGint height)
{
   vgLoadIdentity();
   vgTranslate(x, y);
   vgScale(width, height);
   vgDrawPath(rect, VG_FILL_PATH);
}

static void
test_blend(VGint x, VGint y, VGint width, VGint height)
{
   /* src is red with alpha 0.8 */
   /* dst is green with alpha 0.3 */

   /* 0.8 * red */
   vgSeti(VG_BLEND_MODE, VG_BLEND_SRC);
   rectangle(x, y, width, height);
   x += width + 5;

   /* 0.8 * red + 0.06 * green */
   vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);
   rectangle(x, y, width, height);
   x += width + 5;

   /* 0.56 * red + 0.3 * green */
   vgSeti(VG_BLEND_MODE, VG_BLEND_DST_OVER);
   rectangle(x, y, width, height);
   x += width + 5;

   /* 0.24 * red */
   vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_IN);
   rectangle(x, y, width, height);
   x += width + 5;

   /* 0.24 * green */
   vgSeti(VG_BLEND_MODE, VG_BLEND_DST_IN);
   rectangle(x, y, width, height);
   x += width + 5;

   /* (...) * 0.8 * red + 0.06 * green */
   vgSeti(VG_BLEND_MODE, VG_BLEND_MULTIPLY);
   rectangle(x, y, width, height);
   x += width + 5;

   /* 0.8 * red + (white - 0.8 * red) * 0.3 * green */
   vgSeti(VG_BLEND_MODE, VG_BLEND_SCREEN);
   rectangle(x, y, width, height);
   x += width + 5;

   /* min(SRC_OVER, DST_OVER) */
   vgSeti(VG_BLEND_MODE, VG_BLEND_DARKEN);
   rectangle(x, y, width, height);
   x += width + 5;

   /* max(SRC_OVER, DST_OVER) */
   vgSeti(VG_BLEND_MODE, VG_BLEND_LIGHTEN);
   rectangle(x, y, width, height);
   x += width + 5;
}

static void
draw(void)
{
   const VGfloat white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
   const VGfloat green[4] = { 0.0f, 1.0f, 0.0f, 0.3f };
   VGint x, y;

   vgSetfv(VG_CLEAR_COLOR, 4, white);
   vgClear(0, 0, window_width(), window_height());

   vgSetfv(VG_CLEAR_COLOR, 4, green);

   x = y = 5;
   vgClear(x, y, window_width(), 20);
   test_blend(x, y, 20, 20);

   y += window_height() / 2;
   vgClear(x, y, window_width(), 20);
   /*
    * This will have more green because
    *
    *   result = blended * coverage + dst * (1 - coverage)
    */
   test_blend(x, y, 20, 20);
}


int main(int argc, char **argv)
{
   set_window_size(300, 300);
   set_window_alpha_size(1);
   return run(argc, argv, init, reshape,
              draw, 0);
}
