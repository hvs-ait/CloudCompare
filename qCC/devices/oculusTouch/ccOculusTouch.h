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

#ifndef CC_OCULUS_TOUCH
#define CC_OCULUS_TOUCH

// Qt
#include "qtimer.h"

// Oculus OVR SDK
#include "oculus/ccOculus.h"

// CC
#include "ccGenericGLDisplay.h"
#include "ccMainAppInterface.h"
#include "ccOculusController.h"

// std
#include <math.h>

/* ToDo:
1. done: Use internal QTimer for updating state inside ccOculusTouch
2. done: Toggle between different views when clicking the 'X' button
3. Handle 'viewer-based' rendering mode
4. Improve Oculus touch elements in Stereo dialog
5. Rename Oculus controller parameters in StereoParameters
*/
constexpr unsigned int c_leftHand = 0;
constexpr unsigned int c_rightHand = 1;
constexpr float c_triggerDeadZone = 0.8f;

class ccOculusTouch : public ccOculusController
{

public:
	explicit ccOculusTouch(ccMainAppInterface *appInterface, ovrSession ovrSession, float rotationSpeed, float translationSpeed) :
		ccOculusController(appInterface, ovrSession),
		m_rotationSpeed(rotationSpeed * M_PI / 180.0f),
		m_translationSpeed(translationSpeed / 10.0f),
		m_hasLeftFist(false),
		m_hasRightFist(false),
		m_buttonAPressed(false),
		m_buttonBPressed(false),
		m_buttonXPressed(false),
		m_buttonYPressed(false),
		m_currentView(1)
	{
		m_controllerType = ovrControllerType::ovrControllerType_Touch;
		m_fistPositionOld[0] = ovrVector3f{ 0, 0, 0 };
		m_fistPositionOld[1] = ovrVector3f{ 0, 0, 0 };
		m_timeStamp = ovr_GetTimeInSeconds();
		resetControls();
		ovr_RecenterTrackingOrigin(ovrSession);
		connect(&m_timer, &QTimer::timeout, this, &ccOculusTouch::update);
		m_timer.start(0);

		ccLog::Print("[OculusController] Oculus Touch Controllers initialized!");
	}

protected:
	void applyControls() override;
	void resetControls() override;
signals:
	void update() override;

private:
	ovrVector3f getHandPosition(unsigned int hand);

	void updateButtons(const ovrInputState& inputState);
	void updateGestures(const ovrInputState& inputState);
	void updateThumbSticks(const ovrInputState& inputState, double deltaTime);

	void PreCalculateDoubleHandGestures();
	void PreCalculateXYRotationBasedOnHandPosition(unsigned int hand);
	void PreCalculateTranslationBasedOnHandPosition(unsigned int hand);
	void PreCalculateRotationBasedOnThumbStick(const ovrVector2f *thumbsticks, double deltaTime);
	void PreCalculateTranslationBasedOnThumbStick(const ovrVector2f *thumbstick, double deltaTime);

	double getAngleBetween2dVectors(float aVec1, float bVec1, float aVec2, float bVec2, bool clockWiseRotation) const;

private:
	const float m_rotationSpeed;
	const float m_translationSpeed;

	CCVector3 m_translation;
	bool m_hasTranslation;

	ccGLMatrixd m_rotation;
	bool m_hasRotation;

	ovrVector3f m_fistPositionOld[2];
	bool m_hasRightFist;
	bool m_hasLeftFist;

	bool m_buttonXPressed;
	bool m_buttonYPressed;
	bool m_buttonAPressed;
	bool m_buttonBPressed;

	ccViewportParameters m_viewParameters;

	double m_timeStamp;

	QTimer m_timer;

	unsigned int m_currentView;

	enum CC_VIEWS
	{
		V3DCMD_VIEW_TOP = 1,
		V3DCMD_VIEW_LEFT = 2,
		V3DCMD_VIEW_RIGHT = 3,
		V3DCMD_VIEW_FRONT = 4,
		V3DCMD_VIEW_BOTTOM = 5,
		V3DCMD_VIEW_BACK = 6,
		V3DCMD_VIEW_ISO1 = 7,
		V3DCMD_VIEW_ISO2 = 8,
	};

};

#endif
