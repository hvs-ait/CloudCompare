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
#include "ccGenericGLDisplay.h"
#include <math.h>

constexpr unsigned int c_left_hand = 0;
constexpr unsigned int c_right_hand = 1;
//constexpr float TRANSLATION_SPEED = 1.0f; // in meter/sec
//constexpr float ROTATION_SPEED = 0.34906585039887f; // in rad/sec -> approx 20 degree/sec

class ccOculusTouch : public ccOculusController
{
public:
	explicit ccOculusTouch(ccMainAppInterface *appInterface, ovrSession ovrSession, float rotationSpeed, float translationSpeed) :
		ccOculusController(appInterface, ovrSession),
		m_rotationSpeed(rotationSpeed * M_PI / 180.0f),
		m_translationSpeed(translationSpeed / 10.0f)
	{
		m_controllerType = ovrControllerType::ovrControllerType_Touch;
		handPositionOld = ovrVector3f{ 0, 0, 0 };
		m_hasRightFist = false;
		m_hasLeftFist = false;
		m_buttonA = m_buttonB = m_buttonX = m_buttonY = false;
		redraw = false;
		m_timeStamp = ovr_GetTimeInSeconds();
		resetControls();
		ccLog::Print("Oculus Touch Controller connected!");
	}

	void update() override;

protected:
	void applyControls() override;
	void resetControls() override;

private:
	ovrVector3f getHandPosition(unsigned int hand);

	void updateButtons(const ovrInputState& inputState);
	void updateGestures(const ovrInputState& inputState);
	void updateThumbSticks(const ovrInputState& inputState, double deltaTime);

	void PreCalculateRotationBasedOnHandPosition(unsigned int hand);
	void PreCalculateRotationBasedOnThumbStick(const ovrVector2f *thumbsticks, double deltaTime);
	void PreCalculateTranslationBasedOnThumbStick(const ovrVector2f * thumbstick, double deltaTime);

private:
	const float m_rotationSpeed;
	const float m_translationSpeed;

	CCVector3 m_translation;
	bool m_hasTranslation;

	ccGLMatrixd m_rotation;
	bool m_hasRotation;

	ovrVector3f handPositionOld;
	bool m_hasRightFist;
	bool m_hasLeftFist;

	bool m_buttonX;
	bool m_buttonY;
	bool m_buttonA;
	bool m_buttonB;

	bool redraw;

	ccViewportParameters m_viewParameters;

	double m_timeStamp;
};

#endif
