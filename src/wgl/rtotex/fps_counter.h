//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.h
//	class to calculate frames per second
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

class FPS_COUNTER
{
public:
	void Update(void);									//updates counter - call once per frame
	void Shutdown(void);								//send max, min, average to log
	float GetFps(void)	{		return fps;		}
		
	FPS_COUNTER() : fps(0.0f), lastTime(0.0f), frames(0L), time(0.0f) 
	{}
	~FPS_COUNTER()	{}
	
protected:
	float fps;

	float lastTime;
	long frames;
	float time;
};

#endif	//FPS_COUNTER_H