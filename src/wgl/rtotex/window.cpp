//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW.cpp
//	functions to set up an opengl capable window
//	Downloaded from: www.paulsprojects.net
//	Created:	21st June 2002
//	Modified:	26th August 2002	-	Added Input management
//				3rd September 2002	-	Added WINDOW::MakeCurrent - to restore focus to
//																	window after switch
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include "windows.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "log.h"
#include "window.h"

extern LOG errorLog;

bool WINDOW::Init(char * windowTitle,
				  int newWidth, int newHeight,
				  int newColorBits, int newDepthBits, int newStencilBits,
				  int fullscreenflag)
															//CREATE WINDOW
{
	WNDCLASS wc;											//windows class structure
	DWORD dwExStyle;										//extended style info.
	DWORD dwStyle;											//style info

	//set class's member variables
	title=windowTitle;
	width=newWidth;
	height=newHeight;
	colorBits=newColorBits;
	depthBits=newDepthBits;
	stencilBits=newStencilBits;
	
	//set class's fullscreen flag
	if(fullscreenflag == FULL_SCREEN)
	{
		fullscreen=true;									
	}

	if(fullscreenflag == WINDOWED_SCREEN)
	{
		fullscreen=false;
	}

	if(fullscreenflag == CHOOSE_SCREEN)						//Ask user if fullscreen
	{
		if(MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?","Start FullScreen",MB_YESNO|MB_ICONQUESTION)==IDNO)
		{
			fullscreen=false;								//If answered no
		}
		else
		{
			fullscreen=true;								//if answered yes
		}
	}

	RECT WindowRect;								//grab rect. upper left/lower right values
	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;

	hInstance=		GetModuleHandle(NULL);					//Grab an instance for window
	wc.style=		CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
														//window style: redraw on move, own DC
	wc.lpfnWndProc=	(WNDPROC) WndProc;						//Wndproc handles messages
	wc.cbClsExtra=	0;
	wc.cbWndExtra=	0;										//no extra window data
	wc.hInstance=	hInstance;								//Set the instance
	wc.hIcon=		LoadIcon(NULL, IDI_WINLOGO);			//load default icon
	wc.hCursor=		LoadCursor(NULL, IDC_ARROW);			//Load arrow cursor
	wc.hbrBackground=NULL;									//No background rqd for GL
	wc.lpszMenuName=NULL;									//No menu
	wc.lpszClassName="OpenGL";								//set class name

	if(!RegisterClass(&wc))									//try to register class
	{
		errorLog.OutputError("Failed to register the window class");
		return FALSE;
	}
	else
		errorLog.OutputSuccess("Window Class Registered");

	if(fullscreen)											//try to set up fullscreen?
	{
		DEVMODE dmScreenSettings;							//Device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
															//clear memory
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
									//size of devmode structure
		dmScreenSettings.dmPelsWidth=width;					//selected width
		dmScreenSettings.dmPelsHeight=height;				//selected height
		dmScreenSettings.dmBitsPerPel=colorBits;			//selected bpp
		dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
											//try to set mode.CDS_FULLSCREEN removes start bar
		{
			//If mode fails, give 2 options, quit or run in window
			if(MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",title, MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;							//if "yes", try windowed
			}
			else
			{
				//tell user program is closing
				errorLog.OutputError("Program Closed, As Fullscreen Mode Not Supported.");
				return FALSE;								//exit and return FALSE
			}
		}
	}

	if (fullscreen)											//still fullscreen?
	{
		dwExStyle=WS_EX_APPWINDOW;							//window extended style
		dwStyle=WS_POPUP | WS_VISIBLE;						//window style (no border), visible
		ShowCursor(FALSE);									//hide mouse pointer
	}
	else
	{
		dwExStyle=WS_EX_CLIENTEDGE;							//window extended style(3d look)
		dwStyle=WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE;
									//window style (close button, title bar, border, visible)
	}
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	//adjust window to actual requested size, rather than including borders in size. in fullscreen, no effect

	if(!(hWnd=CreateWindowEx(	dwExStyle,					//extended style for window
								"OpenGL",					//class name
								title,						//window title
								WS_CLIPSIBLINGS |			//required style
								WS_CLIPCHILDREN |			//required style
								dwStyle,					//Selected style
								0, 0,						//window position
								WindowRect.right-WindowRect.left,	//calculate adjusted width
								WindowRect.bottom-WindowRect.top,	//calculate adjusted height
								NULL,						// no parent window
								NULL,						//No Menu
								hInstance,					//Instance
								NULL)))						//Dont pass anything to WM_CREATE
	{
		Shutdown();										//if not set up, reset display
		errorLog.OutputError("Window Creation Error.");
															//pop up error message
		return FALSE;										//return false, to quit program
	}
	else
		errorLog.OutputSuccess("Window Created.");

	//set up pixel format(openGL supporting, RGBA, correct bits
	GLuint pixelFormat;								//holds result after searching for mode match

	//calculate alpha bits
	int alphaBits=0;
	
	if(colorBits==32)
		alphaBits=8;

	static PIXELFORMATDESCRIPTOR pfd=				//pfd tells windows how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),						//size of Pixel format descriptor
		1,													//Version Number
		PFD_DRAW_TO_WINDOW |								//must support window
		PFD_SUPPORT_OPENGL |								//must support opengl
		PFD_DOUBLEBUFFER,									//must support double buffer
		PFD_TYPE_RGBA,										//request RGBA format
		colorBits,											//select colour depth
		0, 0, 0, 0, 0, 0,									//colour bits ignored
		alphaBits,													//alpha buffer bits
		0,													//shift bit ignored
		0,													//no accumulation buffer
		0, 0, 0, 0,											//accumulation bits ignored
		depthBits,											//z buffer bits
		stencilBits,										//stencil buffer bits
		0,													//no auxiliary buffer
		PFD_MAIN_PLANE,										//main drawing layer
		0,													//reserved
		0, 0, 0												//layer masks ignored
	};

	if(!(hDC=GetDC(hWnd)))								//did we get a device context?
	{													//if not
		Shutdown();									//Reset display
		errorLog.OutputError("Can't Create a GL Device context.");
		return FALSE;									//return false, to exit
	}
	else
		errorLog.OutputSuccess("DC Created");
	
	if(!(pixelFormat=ChoosePixelFormat(hDC,&pfd)))	//found a matching pixel format?
	{													//if not
		Shutdown();
		errorLog.OutputError("Can't Find a Suitable PixelFormat.");
		return FALSE;
	}
	else
		errorLog.OutputSuccess("Pixel Format Found.");

	if(!SetPixelFormat(hDC, pixelFormat,&pfd))		//are we able to set pixel format?
	{													//if not
		Shutdown();
		errorLog.OutputError("Can't set the pixelformat.");
		return FALSE;
	}
	else
		errorLog.OutputSuccess("Pixel Format set.");

	if(!(hRC=wglCreateContext(hDC)))					//are we able to get rendering context?
	{													//if not
		Shutdown();
		errorLog.OutputError("Can't create a GL rendering context.");
		return FALSE;
	}
	else
		errorLog.OutputSuccess("GL Rendering Context Created.");

	if(!MakeCurrent())						//are we able to activate rendering context?
	{													//if not
		Shutdown();
		return FALSE;
	}
	else
		errorLog.OutputSuccess("GL Rendering Context Activated.");

	//get pixel format parameters
	static PIXELFORMATDESCRIPTOR finalPfd;
	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &finalPfd);

	//output window parameters
	errorLog.OutputNewline();
	errorLog.OutputSuccess("Window Size: (%d, %d)", width, height);
	errorLog.OutputSuccess("Color Buffer Bits (R, G, B, A): (%d, %d, %d, %d)",
								finalPfd.cRedBits,
								finalPfd.cGreenBits,
								finalPfd.cBlueBits,
								finalPfd.cAlphaBits);
	errorLog.OutputSuccess("Depth Buffer Bits: %d", finalPfd.cDepthBits);
	errorLog.OutputSuccess("Stencil Buffer Bits: %d", finalPfd.cStencilBits);
	errorLog.OutputNewline();
	
	ShowWindow(hWnd,SW_SHOW);							//show window
	SetForegroundWindow(hWnd);							//slightly higher priority
	SetFocus(hWnd);										//Set keyboard focus to the window
	
	errorLog.OutputSuccess("Window Created!");
	errorLog.OutputNewline();

	//Init the font
	HFONT font;											//windows font ID

	//create 96 display lists
	base = glGenLists(96);

	font	=	CreateFont(	-18,						//font height
							0,							//default width
							0, 0,						//angles - escapement, orientation
							FW_BOLD,					//font weight, 0-1000, NORMAL, BOLD
							false,						//italic
							false,						//underline
							false,						//strikeout
							ANSI_CHARSET,				//character set
							OUT_TT_PRECIS,				//precision
							CLIP_DEFAULT_PRECIS,		//clip precision
							ANTIALIASED_QUALITY,		//output quality
							FF_DONTCARE | DEFAULT_PITCH,//family and pitch
							"Courier New");				//font name

	//select the font
	SelectObject(hDC, font);

	//create 96 display lists, starting at 32
	wglUseFontBitmaps(hDC, 32, 96, base);
	
	errorLog.OutputSuccess("Font created successfully.");
	
	return TRUE;										//success!
}

void WINDOW::Shutdown(void)									//PROPERLY KILL WINDOW
{
	//Delete font display lists
	glDeleteLists(base, 96);

	if (fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);						//restore desktop mode
		ShowCursor(TRUE);									//show mouse cursor
	}
	
	errorLog.OutputNewline();
		
	if(hRC)													//have a rendering context?
	{
		if(!wglMakeCurrent(NULL, NULL))					//try to release rend cont
		{
			errorLog.OutputError("Release of DC and RC Failed.");
		}
		else
			errorLog.OutputSuccess("DC and RC released.");

		if(!wglDeleteContext(hRC))							//try to delete RC
		{
			errorLog.OutputError("Release Rendering Context Failed.");
		}
		else
			errorLog.OutputSuccess("Rendering Context Released.");

		hRC=NULL;											//set RC to NULL
	}

	if(hDC && !ReleaseDC(hWnd, hDC))						//Are we able to release DC?
	{
		errorLog.OutputError("Release of Device Context Failed.");
		hDC=NULL;
	}
	else
		errorLog.OutputSuccess("Device Context Released.");

	if(hWnd && !DestroyWindow(hWnd))						//Can we destroy window?
	{
		errorLog.OutputError("Could not release hWnd");
		hWnd=NULL;
	}
	else
		errorLog.OutputSuccess("hWnd released.");

	if (!UnregisterClass("OpenGL", hInstance))				 //can we unreg. class?
	{
		errorLog.OutputError("Could Not Unregister Class.");
		hInstance=NULL;
	}
	else
		errorLog.OutputSuccess("Class unregistered.");
}

bool WINDOW::MakeCurrent()
{
	if(!wglMakeCurrent(hDC, hRC))
	{
		errorLog.OutputError("Unable to change current context");
		return false;
	}

	return true;
}


//DEAL WITH ALL WINDOW MESSAGES
//Message Pump
bool WINDOW::HandleMessages(void)
{
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))		//Is there a message waiting?
	{
		if(msg.message==WM_QUIT)
			return false;						//if a quit message, return false

		//handle input
		if(msg.message==WM_KEYDOWN)
			SetKeyPressed(msg.wParam);

		if(msg.message==WM_KEYUP)
			SetKeyReleased(msg.wParam);

		if(msg.message==WM_LBUTTONDOWN)
			SetLeftButtonPressed();
		
		if(msg.message==WM_RBUTTONDOWN)
			SetRightButtonPressed();
		
		TranslateMessage(&msg);					//Translate Message
		DispatchMessage(&msg);					//dispatch message
	}
	return true;
}

LRESULT CALLBACK WINDOW::WndProc(	HWND	hWnd,				//handle for this window
									UINT	uMsg,				//Message for this window
									WPARAM	wParam,				//Additional message information
									LPARAM	lParam)				//Additional Message information

{
	switch	(uMsg)										//check for windows messages
	{
		case WM_SYSCOMMAND:								//Intercept system commands
		{
			switch (wParam)								//check system calls
			{
				case SC_SCREENSAVE:						//screensaver trying to start?
				case SC_MONITORPOWER:					//monitor trying to enter powersave?
				return 0;								//prevent from happening
			}
			break;
		}

		case WM_CLOSE:									//receive close message?
			PostQuitMessage(0);							//send quit message
			return 0;									//quit back
			break;
	}

	//pass all unhandled messages to DefWindowProc, windows can handle
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void WINDOW::SwapBuffers(void)
{
	::SwapBuffers(hDC);					//swap buffers
}

//void WINDOW::CheckGLError - check for an opengl error
void WINDOW::CheckGLError(void)
{
	GLenum error;
	error=glGetError();
	if(!(error==GL_NO_ERROR))
	{
		errorLog.OutputError("OpenGL Error:");
		if(error==GL_INVALID_ENUM)
		{
			errorLog.OutputError("	GL_INVALID_ENUM");
			errorLog.OutputError("	GLenum Argument out of range.");
		}
		if(error==GL_INVALID_VALUE)
		{
			errorLog.OutputError("	GL_INVALID_VALUE");
			errorLog.OutputError("	Numeric Argument out of range.");
		}
		if(error==GL_INVALID_OPERATION)
		{
			errorLog.OutputError("	GL_INVALID_OPERATION");
			errorLog.OutputError("	Invalid Operation in current state.");
		}
		if(error==GL_STACK_UNDERFLOW)
		{
			errorLog.OutputError("	GL_STACK_UNDERFLOW");
			errorLog.OutputError("	Stack Underflow.");
		}
		if(error==GL_STACK_OVERFLOW)
		{
			errorLog.OutputError("	GL_STACK_OVERFLOW");
			errorLog.OutputError("	Stack Overflow.");
		}
		if(error==GL_OUT_OF_MEMORY)
		{
			errorLog.OutputError("	GL_OUT_OF_MEMORY");
			errorLog.OutputError("	Out of memory.");
		}
	}
}

void WINDOW::SaveScreenshot(void)
{
	FILE * file;

	//first calculate the filename to save to
	char filename[32];

	for(int i=0; i<1000; i++)
	{
		sprintf(filename, "screen%03d.tga", i);
		
		//try opening this file - if not possible, use this filename
		file=fopen(filename, "rb");

		if(!file)
		{
			break;
		}
		
		//otherwise, the file exists, try next, except if this is the last one
		fclose(file);

		if(i==999)
		{
			errorLog.OutputError("No space to save screenshot - 0-999 exist");
			return;
		}
	}

	errorLog.OutputSuccess("Saving %s", filename);
	
	GLubyte		TGAheader[12]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};	//Uncompressed TGA header
	GLubyte		infoHeader[6];

	unsigned char * data=new unsigned char[4*width*height];
	if(!data)
	{
		errorLog.OutputError("Unable to allocate memory for screen data");
		return;
	}

	//read in the screen data
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//data needs to be in BGR format
	//swap b and r
	for(int i=0; i<(int)width*height*4; i+=4)
	{	
		//repeated XOR to swap bytes 0 and 2
		data[i] ^= data[i+2] ^= data[i] ^= data[i+2];
	}
	
	//open the file
	file = fopen(filename, "wb");

	//save header
	fwrite(TGAheader, 1, sizeof(TGAheader), file);

	//save info header
	infoHeader[0]=(width & 0x00FF);
	infoHeader[1]=(width & 0xFF00) >> 8;
	infoHeader[2]=(height & 0x00FF);
	infoHeader[3]=(height & 0xFF00) >> 8;
	infoHeader[4]=32;
	infoHeader[5]=0;

	//save info header
	fwrite(infoHeader, 1, sizeof(infoHeader), file);

	//save the image data
	fwrite(data, 1, width*height*4, file);
	
	fclose(file);
	
	errorLog.OutputSuccess("Saved Screenshot: %s", filename);
	return;
}

//Text writing functions
void WINDOW::StartTextMode(void)
{
	//If not yet created, make display list
	if(!startTextModeList)
	{
		startTextModeList=glGenLists(1);
		glNewList(startTextModeList, GL_COMPILE);
		{
			//save states
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glListBase(base-32);							//set the list base
		
			//set modelview matrix
			glPushMatrix();
			glLoadIdentity();

			//set projection matrix
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
	
			//set states
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBlendFunc(GL_ONE, GL_ONE);	//if blending, use additive
		}
		glEndList();
	}

	glCallList(startTextModeList);
}

void WINDOW::Print(int x, int y, const char * string, ...)
{
	char text[256];									//Holds our string
	va_list va;										//pointer to list of arguments
	
	if(string==NULL)								//If there's no text
		return;										//Do nothing

	va_start(va, string);							//parse string for variables
		vsprintf(text, string, va);					//convert to actual numbers
	va_end(va);										//results stored in text

	glRasterPos2i(x, y);								//go to correct raster position

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	//call display lists
}

void WINDOW::EndTextMode(void)
{
	//restore states
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}

//Update input
void WINDOW::Update()
{
	//Mouse buttons are marked as pressed by windows messages
	//see if any have been released
	if(mouseLDown && !GetAsyncKeyState(VK_LBUTTON))
		mouseLDown=false;

	if(mouseRDown && !GetAsyncKeyState(VK_RBUTTON))
		mouseRDown=false;

	//Update the mouse position
	static POINT mousePosition;
	GetCursorPos(&mousePosition);

	oldMouseX=mouseX;
	oldMouseY=mouseY;

	mouseX=mousePosition.x;
	mouseY=mousePosition.y;
}
