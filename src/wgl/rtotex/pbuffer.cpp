//////////////////////////////////////////////////////////////////////////////////////////
//	PBUFFER.cpp
//	functions to set up a pbuffer
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glu.h>
#include "log.h"
#include "pbuffer.h"

extern LOG errorLog;

bool PBUFFER::Init(	int newWidth, int newHeight,
					int newColorBits, int newDepthBits, int newStencilBits,
					int numExtraIAttribs, int * extraIAttribList, int * flags)
{
	//Check for pbuffer support
	if(	!WGLEW_ARB_extensions_string ||
		!WGLEW_ARB_pixel_format ||
		!WGLEW_ARB_pbuffer)
	{
		errorLog.OutputError("Extension required for pbuffer unsupported");
		return false;
	}

	//set class's member variables
	width=newWidth;
	height=newHeight;
	colorBits=newColorBits;
	depthBits=newDepthBits;
	stencilBits=newStencilBits;

	//Get the current device context
	HDC hCurrentDC=wglGetCurrentDC();
	if(!hCurrentDC)
	{
		errorLog.OutputError("Unable to get current Device Context");
		return false;
	}
	

	//choose pixel format
	GLint pixelFormat;

	const int standardIAttribList[]={	WGL_DRAW_TO_PBUFFER_ARB, 1,
										WGL_COLOR_BITS_ARB, colorBits,
										WGL_ALPHA_BITS_ARB, colorBits==32 ? 8 : 0,
										WGL_DEPTH_BITS_ARB, depthBits,
										WGL_STENCIL_BITS_ARB, stencilBits};
	const float fAttribList[]={	
										0};

	//add the extraIAttribList to the standardIAttribList
	int * iAttribList=new int[sizeof(standardIAttribList)/sizeof(int)+numExtraIAttribs*2+1];
	if(!iAttribList)
	{
		errorLog.OutputError("Unable to allocate space for iAttribList");
		return false;
	}

	memcpy(	iAttribList, standardIAttribList, sizeof(standardIAttribList));
	memcpy( iAttribList+sizeof(standardIAttribList)/sizeof(int),
			extraIAttribList, numExtraIAttribs*2*sizeof(int)+sizeof(int));

	//Choose pixel format
	unsigned int numFormats;
	if(!wglChoosePixelFormatARB(hCurrentDC, iAttribList, fAttribList, 1,
								&pixelFormat, &numFormats))
	{
		errorLog.OutputError("Unable to find a pixel format for the pbuffer");
		return false;
	}

	//Create the pbuffer
	hBuffer=wglCreatePbufferARB(hCurrentDC, pixelFormat, width, height, flags);
	if(!hBuffer)
	{
		errorLog.OutputError("Unable to create pbuffer");
		return false;
	}

	//Get the pbuffer's device context
	hDC=wglGetPbufferDCARB(hBuffer);
	if(!hDC)
	{
		errorLog.OutputError("Unable to get pbuffer's device context");
		return false;
	}

	//Create a rendering context for the pbuffer
	hRC=wglCreateContext(hDC);
	if(!hRC)
	{
		errorLog.OutputError("Unable to create pbuffer's rendering context");
		return false;
	}

	//Set and output the actual pBuffer dimensions
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_WIDTH_ARB, &width);
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_HEIGHT_ARB, &height);
	errorLog.OutputSuccess("Pbuffer Created: (%d x %d)", width, height);
	
	return TRUE;										//success!
}

void PBUFFER::Shutdown(void)
{
	if(hRC)													//have a rendering context?
	{
		if(!wglDeleteContext(hRC))							//try to delete RC
		{
			errorLog.OutputError("Release of Pbuffer Rendering Context Failed.");
		}
		
		hRC=NULL;											//set RC to NULL
	}

	if(hDC && !wglReleasePbufferDCARB(hBuffer, hDC))		//Are we able to release DC?
	{
		errorLog.OutputError("Release of Pbuffer Device Context Failed.");
		hDC=NULL;
	}
	
	if(!wglDestroyPbufferARB(hBuffer))
	{
		errorLog.OutputError("Unable to destroy pbuffer");
	}
}


bool PBUFFER::MakeCurrent()
{
	if(!wglMakeCurrent(hDC, hRC))
	{
		errorLog.OutputError("Unable to change current context");
		return false;
	}

	return true;
}

