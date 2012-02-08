//////////////////////////////////////////////////////////////////////////////////////////
//	INTERACTOR.h
//	Class declaration for mouse interactor class
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef INTERACTOR_H
#define INTERACTOR_H

class INTERACTOR
{
public:
	void Init(	VECTOR3D startPosition,
				float newMinR=0.0f, float newMaxR=10000000.0f,
				VECTOR3D newLookAt=VECTOR3D(0.0f, 0.0f, 0.0f));
	
	void SetSensitivity(double newRotationSensitivity=0.5, double newTranslationSensitivity=0.01)
	{
		rotationSensitivity=newRotationSensitivity;
		translationSensitivity=newTranslationSensitivity;
	}
	
	virtual void Update();
	void SetupViewMatrix();

	INTERACTOR()	:	rotationSensitivity(0.5), translationSensitivity(0.01)
	{}
	virtual ~INTERACTOR()
	{}

	VECTOR3D position;
	VECTOR3D lookAt;

	MATRIX4X4 viewMatrix;

	//spherical coordinates
	float r;
	double leftRightRotation, upDownRotation;
	
	double rotationSensitivity, translationSensitivity;
	float minR, maxR;
};

#endif	//INTERACTOR_H