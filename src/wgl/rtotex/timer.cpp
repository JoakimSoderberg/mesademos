//////////////////////////////////////////////////////////////////////////////////////////
//	TIMER.cpp
//	functions for timer
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include "timer.h"

void TIMER::Reset()
{
	startTime=(double)timeGetTime();
}

double TIMER::GetTime()
{
	if(isPaused)
		return pauseTime-startTime;
	else
		return ((double)timeGetTime())-startTime;
}

void TIMER::Pause()
{
	if(isPaused)
		return;		//only pause if unpaused

	isPaused=true;
	pauseTime=(double)timeGetTime();
}

void TIMER::Unpause()
{
	if(!isPaused)
		return;		//only unpause if paused

	isPaused=false;
	startTime+=((double)timeGetTime()-pauseTime);	//update start time to reflect pause
}
