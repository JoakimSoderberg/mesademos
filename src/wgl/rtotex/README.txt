**************************************************************************
**	Render to texture
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This is my first use of pbuffers. It is a simple project which creates a render-to-texture-enabled pbuffer, and a texture object related to it. When rendering, the pbuffer is made the current context, and two wire tori are rendered into it. The window is then made current, and the pbuffer is used to texture a single rectangle. The camera can be moved around this rectangle.

If the SGIS_generate_mipmap extension is supported, it is used to automatically generate mipmaps for the texture each time it is updated, and a LINEAR_MIPMAP_LINEAR filter is used. Otherwise, a LINEAR filter is used.

If anisotropic filtering is supported, the maximum degree of anisotropy is enabled at start-up. The anisotropy level can be altered to show the difference this filtering has on the textured quad.

It is also possible to render a second scene into the pbuffer, a textured teapot instead of the wire torus. This demonstrates rendering using textures in the pbuffer, then using the pbuffer image itself as a texture. The important thing to note here is that the texture object which will use the pbuffer image is part of the window context. The image on the teapot is part of the pbuffer context. It is important to load the image when the pbuffer context is current since in this demo, the two contexts do not share textures.


Requirements:

WGL_ARB_extensions_string
WGL_ARB_pbuffer
WGL_ARB_pixel_format
WGL_ARB_render_texture

Optional:

EXT_texture_filter_anisotropic
SGIS_generate_mipmap


References:

"Using pbuffers for off-screen rendering", Chris Wynn. From developer.nvidia.com
"OpenGL Render-to-Texture", Chris Wynn. From developer.nvidia.com
Extension specifications for the above WGL extensions.


Keys:

F1	-	Take a screenshot
Escape	-	Quit

1	-	Draw wire torus shape
2	-	Draw textured teapot

Up	-	Increase texture max anisotropy (if anisotropic filtering is supported)
Down	-	Decrease texture max anisotropy (if anisotropic filtering is supported)

M	-	Use LINEAR_MIPMAP_LINEAR filtering, if SGIS_generate_mipmap is supported
L	-	Use LINEAR filtering

P	-	Pause
U	-	Unpause
