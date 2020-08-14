//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: AIT-Austrian Institute of Technology               #
//#                                                                        #
//##########################################################################

#ifndef CC_OOCULUS_TOUCH
#define CC_OOCULUS_TOUCH

#include "oculus/ccOculus.h"
#include "ccOculusController.h"

constexpr unsigned int LEFT_HAND = 0;
constexpr unsigned int RIGHT_HAND = 1;
constexpr float TRANSLATION_SPEED = 10.0;
constexpr float ROTATION_SPEED = 0.005;

class ccOculusTouch : public ccOculusController
{
public:
	ccOculusTouch(ccMainAppInterface *appInterface, ovrSession ovrSession) : 
		ccOculusController(appInterface, ovrSession) {}

	void initController() override;
	void update() override;

protected:
	void applyControls() override;
	void resetControls() override;

private:
	ovrVector3f getHandPosition(unsigned int hand);
	void rotateBasedOnHandPosition(unsigned int hand);

private:

	CCVector3 m_translation;
	bool m_hasTranslation;

	ccGLMatrixd m_rotation;
	bool m_hasRotation;

	bool m_hasRightFist;
	bool m_hasRightFistReleased;

	bool m_hasLeftFist;
	bool m_hasLeftFistReleased;

	ovrVector3f handPositionOld;
};

#endif
