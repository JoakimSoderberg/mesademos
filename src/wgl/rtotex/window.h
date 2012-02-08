//////////////////////////////////////////////////////////////////////////////////////////
//	window.h
//	class to setup opengl capable window
//	Downloaded from: www.paulsprojects.net
//	Created:	21st June 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WINDOW_H
#define WINDOW_H

class WINDOW
{
public:
	HGLRC hRC;													//Permanent rendering context
	HDC hDC;													//Private GDI device context
	HWND hWnd;													//Holds window handle
	HINSTANCE hInstance;										//Holds application instance
	MSG msg;								//message structure, to see if any pending messages
	char * title;												//Window Title
	int width, height;											//window size
	int colorBits, depthBits, stencilBits;						//window bpp
	bool fullscreen;											//fullscreen?
	
	bool Init(	char * windowTitle,
				int newWidth, int newHeight,
				int newColorBits, int newDepthBits, int newStencilBits,
				int fullscreenflag);
																//Create Window
	void Shutdown(void);										//Properly kill window

	bool MakeCurrent(void);

	bool HandleMessages(void);
	static LRESULT CALLBACK WndProc(	HWND	hWnd,
										UINT	uMsg,
										WPARAM	wParam,
										LPARAM	lParam);		//DEAL WITH ALL WINDOW MESSAGES
																//static needed for CALLBACK
	void SwapBuffers();
	void CheckGLError(void);									//check for an OpenGL error

	void SaveScreenshot(void);


	//Text writing functions
	void StartTextMode(void);
	void Print(int x, int y, const char * string, ...);
	void EndTextMode(void);

	protected:
	GLuint base;					//display list base for font
	GLuint startTextModeList;



	//INPUT FUNCTIONS
	public:
	void Update();

	//KEYBOARD
	void SetKeyPressed(int keyNumber)
	{	keyPressed[keyNumber]=true;		}
	
	void SetKeyReleased(int keyNumber)
	{	keyPressed[keyNumber]=false;	}

	bool isKeyPressed(int keyNumber)
	{	return keyPressed[keyNumber];	}

	protected:
	bool keyPressed[256];
	
	//MOUSE
	public:
	void SetLeftButtonPressed()
	{	mouseLDown=true;	}
	
	void SetRightButtonPressed()
	{	mouseRDown=true;	}
	
	bool isLeftButtonPressed()
	{	return mouseLDown;	}

	bool isRightButtonPressed()
	{	return mouseRDown;	}

	int GetMouseX()
	{	return mouseX;		}

	int GetMouseY()
	{	return mouseY;		}

	int GetMouseXMovement()
	{	return mouseX-oldMouseX;	}

	int GetMouseYMovement()
	{	return mouseY-oldMouseY;	}

	protected:
	int oldMouseX, oldMouseY;
	int mouseX, mouseY;
	bool mouseLDown, mouseRDown;




	public:
	WINDOW() : title("Paul's Projects"), fullscreen(TRUE)
	{}
	~WINDOW()	{}
};

//#defines for window or fullscreen
#define WINDOWED_SCREEN		0
#define FULL_SCREEN			1
#define CHOOSE_SCREEN		2

#endif	//WINDOW_H