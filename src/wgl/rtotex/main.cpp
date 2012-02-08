//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Render to texture
//	Downloaded from: www.paulsprojects.net
//	Created:	10th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "log.h"
#include "window.h"
#include "fps_counter.h"
#include "timer.h"
#include "maths/maths.h"
#include "pbuffer.h"
#include "interactor.h"
#include "image.h"
#include "main.h"

//errorLog MUST be kept - it is used by other files
LOG errorLog;
WINDOW window;
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR windowBackgroundColor(0.0f, 0.0f, 1.0f, 0.0f);
COLOR pbufferBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

INTERACTOR camera;

int pbufferSize=512;
PBUFFER pbuffer;
GLuint pbufferTexture;	//texture object which is used when pbuffer texture is displayed

//anisotropy
int currentAnisotropy=1.0f;
int maxAnisotropy=1.0f;

//Using mipmap filtering?
bool useMipmapFilter=false;

//Draw the textured teapot into the pbuffer?
bool drawTextured=false;

GLuint decalTexture;	//texture ID

//Set up variables
bool DemoInit()
{
	if(!window.Init("Render To Texture", 640, 480, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created

	glewInit();

	camera.Init(VECTOR3D(0.0f, 0.0f, -2.5f), 2.0f, 100.0f);

	//Set up extensions
	if(	!WGLEW_ARB_extensions_string)
		return false;

	//Set up wgl extensions
	if(	!WGLEW_ARB_pbuffer || !WGLEW_ARB_pixel_format ||
		!WGLEW_ARB_render_texture)
		return false;


	//Init the pbuffer
	int pbufferExtraIAttribs[]={WGL_BIND_TO_TEXTURE_RGBA_ARB, true,
								0};

	int pbufferFlags[]={WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
						WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
						
						//request mipmap space if mipmaps are to be used
						GLEW_SGIS_generate_mipmap ? WGL_MIPMAP_TEXTURE_ARB : 0,
						GLEW_SGIS_generate_mipmap ? true : 0,

						0};

	if(!pbuffer.Init(pbufferSize, pbufferSize, 32, 24, 8, 1, pbufferExtraIAttribs, pbufferFlags))
		return false;
	

	//Create the texture object to relate to the pbuffer
	glGenTextures(1, &pbufferTexture);
	glBindTexture(GL_TEXTURE_2D, pbufferTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Use generated mipmaps if supported
	if(GLEW_SGIS_generate_mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
		useMipmapFilter=true;
	}

	//Use maximum anisotropy if supported
	if(GLEW_EXT_texture_filter_anisotropic)
	{
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		currentAnisotropy=maxAnisotropy;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, currentAnisotropy);
	}



	//Load the decal texture
	//Note: This MUST be done when the pbuffer is the current context
	pbuffer.MakeCurrent();
	
	IMAGE decalImage;
	decalImage.Load("decal.bmp");

	glGenTextures(1, &decalTexture);
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, decalImage.width, decalImage.height,
					0, decalImage.format, GL_UNSIGNED_BYTE, decalImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	
	//reset timer for start
	timer.Reset();
	
	return true;
}

//Set up openGL
bool GLInit()
{
	//Set up for pbuffer
	pbuffer.MakeCurrent();

	//set viewport
	glViewport(0, 0, pbufferSize, pbufferSize);

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
	
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	pbufferBackgroundColor.r,
					pbufferBackgroundColor.g,
					pbufferBackgroundColor.b,
					pbufferBackgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);





	//Set up for window
	window.MakeCurrent();

	//set viewport
	int height;
	if (window.height==0)
		height=1;
	else
		height=window.height;
	
	glViewport(0, 0, window.width, height);					//reset viewport

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(45.0f, (GLfloat)window.width/(GLfloat)height, 1.0f, 100.0f);
	
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	windowBackgroundColor.r,
					windowBackgroundColor.g,
					windowBackgroundColor.b,
					windowBackgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);


	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();
	camera.Update();

	//Change anisotropy level
	if(	window.isKeyPressed(VK_UP) && GLEW_EXT_texture_filter_anisotropic &&
		currentAnisotropy<maxAnisotropy)
	{
		window.MakeCurrent();
		currentAnisotropy*=2;
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, currentAnisotropy);
		window.SetKeyReleased(VK_UP);
	}
	
	if(	window.isKeyPressed(VK_DOWN) && GLEW_EXT_texture_filter_anisotropic &&
		currentAnisotropy>1)
	{
		window.MakeCurrent();
		currentAnisotropy/=2;
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, currentAnisotropy);
		window.SetKeyReleased(VK_DOWN);
	}

	//toggle mipmaps
	if( window.isKeyPressed('M') && useMipmapFilter==false && GLEW_SGIS_generate_mipmap)
	{
		window.MakeCurrent();
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, true);
		useMipmapFilter=true;
	}

	if( window.isKeyPressed('L') && useMipmapFilter==true && GLEW_SGIS_generate_mipmap)
	{
		window.MakeCurrent();
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, false);
		useMipmapFilter=false;
	}

	//Pause/unpause
	if(window.isKeyPressed('P'))
		timer.Pause();

	if(window.isKeyPressed('U'))
		timer.Unpause();

	//Swap between scenes in the pbuffer
	if(window.isKeyPressed('1') && drawTextured)
	{
		//Draw wire tori
		drawTextured=false;
	}

	if(window.isKeyPressed('2') && !drawTextured)
	{
		//draw textured sphere
		drawTextured=true;
	}
}

//draw a frame
void RenderFrame()
{
	//Draw to pbuffer
	pbuffer.MakeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	gluLookAt(	0.0f, 0.0f, 4.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Draw scene
	if(drawTextured)
	{
		glBindTexture(GL_TEXTURE_2D, decalTexture);
		glEnable(GL_TEXTURE_2D);
		
		glPushMatrix();
		glRotatef(timer.GetTime()/20, 0.0f, 1.0f, 0.0f);

		glutSolidTeapot(0.8f);
		
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glPushMatrix();
		glRotatef(timer.GetTime()/20, 0.0f, 1.0f, 0.0f);
		glRotatef(55.0f, 1.0f, 0.0f, 0.0f);
		glutWireTorus(0.3f, 1.0f, 12, 24);
		glPopMatrix();

		glPushMatrix();
		glRotatef(timer.GetTime()/20, 0.0f, 1.0f, 0.0f);
		glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
		glutWireTorus(0.3f, 1.0f, 12, 24);
		glPopMatrix();
	}


	
	//Draw to window
	window.MakeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.SetupViewMatrix();
	glLoadMatrixf(camera.viewMatrix);


	glBindTexture(GL_TEXTURE_2D, pbufferTexture);
	//use the pbuffer as the texture
	wglBindTexImageARB(pbuffer.hBuffer, WGL_FRONT_LEFT_ARB);


	//Draw simple rectangle
	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 0.0f);
	}
	glEnd();

	//release the pbuffer for further rendering
	wglReleaseTexImageARB(pbuffer.hBuffer, WGL_FRONT_LEFT_ARB);



	fpsCounter.Update();											//update frames per second counter
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	window.Print(0, 48, "%dx Anisotropy", currentAnisotropy);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	window.Print(0, 68, "%s", useMipmapFilter ?	"LINEAR_MIPMAP_LINEAR filtering" :
												"LINEAR filtering");
	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if(window.isKeyPressed(VK_F1))
	{
		window.SaveScreenshot();
		window.SetKeyReleased(VK_F1);
	}

	window.SwapBuffers();									//swap buffers

	//check for any opengl errors
	window.CheckGLError();

	//quit if necessary
	if(window.isKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);
}

void DemoShutdown()
{
	pbuffer.Shutdown();

	window.Shutdown();										//Shutdown window
}

//ENTRY POINT FOR APPLICATION
//CALL WINDOW CREATION ROUTINE, DEAL WITH MESSAGES, WATCH FOR INTERACTION
int WINAPI WinMain(	HINSTANCE	hInstance,				//instance
					HINSTANCE	hPrevInstance,			//Previous Instance
					LPSTR		lpCmdLine,				//command line parameters
					int			nCmdShow)				//Window show state
{
	//Initiation
	errorLog.Init("Error Log.txt");

	//init variables etc, then GL
	if(!DemoInit())
	{
		errorLog.OutputError("Demo Initiation failed");
		return 0;
	}
	else
		errorLog.OutputSuccess("Demo Initiation Successful");

	if(!GLInit())
	{
		errorLog.OutputError("OpenGL Initiation failed");
		return 0;
	}
	else
		errorLog.OutputSuccess("OpenGL Initiation Successful");

	//Main Loop
	for(;;)
	{
		if(!(window.HandleMessages())) break;//handle windows messages, quit if returns false
		UpdateFrame();
		RenderFrame();
	}

	DemoShutdown();
	
	errorLog.OutputSuccess("Exiting...");
	return (window.msg.wParam);								//Exit The Program
}
