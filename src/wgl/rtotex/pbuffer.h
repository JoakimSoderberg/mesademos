//////////////////////////////////////////////////////////////////////////////////////////
//	PBUFFER.h
//	class to setup pBuffer
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef PBUFFER_H
#define PBUFFER_H

class PBUFFER
{
public:
	HGLRC hRC;													//rendering context
	HDC hDC;													//device context

	HPBUFFERARB hBuffer;										//buffer handle

	int width, height;											//window size
	int colorBits, depthBits, stencilBits;						//window bpp
	
	bool Init(	int newWidth, int newHeight,
				int newColorBits, int newDepthBits, int newStencilBits,
				int numExtraIAttribs, int * extraIAttribList, int * flags);
	void Shutdown(void);										//Properly kill pbuffer

	bool MakeCurrent(void);
};

#endif	//PBUFFER_H