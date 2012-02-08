//////////////////////////////////////////////////////////////////////////////////////////
//	INTERACTOR.h
//	Functions for mouse interactor class
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Updated:	30th August 2002	-	Added bounds on r
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL/gl.h>
#include "log.h"
#include "window.h"
#include "maths/maths.h"
#include "interactor.h"

extern WINDOW window;
extern LOG errorLog;

void INTERACTOR::Init(	VECTOR3D startPosition, float newMinR, float newMaxR,
						VECTOR3D newLookAt)
{
	position=startPosition;
	lookAt=newLookAt;

	minR=newMinR;
	maxR=newMaxR;
	
	//calculate r and angles
	VECTOR3D lineOfSight=position-lookAt;
	r=lineOfSight.GetLength();
	
	if(lineOfSight.z==0.0f)
	{
		if(lineOfSight.x>0.0f)
			leftRightRotation=-90.0f;
		if(lineOfSight.x<0.0f)
			leftRightRotation=90.0f;
	}
	else
	{
		if(lineOfSight.z<0.0f)
			leftRightRotation=180.0-(180.0/M_PI)*atan(lineOfSight.x/lineOfSight.z);
		if(lineOfSight.z>0.0f)
			leftRightRotation=-(180.0/M_PI)*atan(lineOfSight.x/lineOfSight.z);
	}


	
	if(lineOfSight.x==0.0f && lineOfSight.z==0.0f)
		upDownRotation=90.0;
	else
		upDownRotation=(180.0/M_PI)*atan(lineOfSight.y/sqrt((lineOfSight.x*lineOfSight.x)+
															(lineOfSight.z*lineOfSight.z)));
}

void INTERACTOR::Update()
{
	//update angles
	if(window.isLeftButtonPressed())
	{
		leftRightRotation+=(window.GetMouseXMovement())*rotationSensitivity;
		upDownRotation+=(window.GetMouseYMovement())*rotationSensitivity;
	}

	//update radius
	if(window.isRightButtonPressed())
	{
		//move out
		if(window.GetMouseYMovement()>0)
		{
			//move out no further than maxR
			if(r+(window.GetMouseYMovement())*(float)translationSensitivity>maxR)
				r=maxR;
			else
				r+=(window.GetMouseYMovement())*(float)translationSensitivity;
		}

		//move in
		if(window.GetMouseYMovement()<0)
		{
			//move in no further than minR
			if(r+(window.GetMouseYMovement())*(float)translationSensitivity<minR)
				r=minR;
			else
				r+=(window.GetMouseYMovement())*(float)translationSensitivity;
		}
	}

	//update position if there has been a change
	if(window.isLeftButtonPressed() || window.isRightButtonPressed())
	{
		static MATRIX4X4 tempMatrix;
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(lookAt.x, lookAt.y, lookAt.z);
		glRotated(-leftRightRotation, 0.0f, 1.0f, 0.0f);
		glRotated(-upDownRotation, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, r);
		glGetFloatv(GL_MODELVIEW_MATRIX, tempMatrix);
		position=tempMatrix.GetColumn(3);
		glPopMatrix();
	}
}

void INTERACTOR::SetupViewMatrix()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -r);
	glRotated(upDownRotation, 1.0f, 0.0f, 0.0f);
	glRotated(leftRightRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(-lookAt.x, -lookAt.y, -lookAt.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	glPopMatrix();
}
