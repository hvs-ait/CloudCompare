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

#include <QAction>
#include <QMainWindow>
#include <QMenu>

#include "ccOculusTouch.h"
#include "ccGLWindow.h"

#include "ccMainAppInterface.h"
#include <algorithm>

void ccOculusTouch::initController()
{
	m_controllerType = ovrControllerType::ovrControllerType_Touch;
	resetControls();
	handPositionOld = ovrVector3f{ 0, 0, 0 };
	m_hasLeftFistReleased = true;
	m_hasRightFistReleased = true;
	m_hasRightFist = false;
	m_hasLeftFist = false;
	ccLog::Print("Oculus Touch Controller connected!");
}

void ccOculusTouch::update()
{
	ovrInputState inputState;
	ovr_GetInputState(m_ovrSession, m_controllerType, &inputState);

	ovrVector2f *thumbsticks = inputState.Thumbstick;

	m_hasLeftFist = (inputState.HandTrigger[LEFT_HAND] >= 0.8 && inputState.IndexTrigger[LEFT_HAND] >= 0.8);
	m_hasRightFist = (inputState.HandTrigger[RIGHT_HAND] >= 0.8 && inputState.IndexTrigger[RIGHT_HAND] >= 0.8);
	
	if (m_hasLeftFist && m_hasRightFist) {
		// Zoom - Not implemented
		m_hasLeftFistReleased = m_hasRightFistReleased = false;
	}

	else if (m_hasLeftFist && !m_hasRightFist) {
		if (m_hasLeftFistReleased) {
			m_hasLeftFistReleased = false;
			m_hasRightFistReleased = true;
			handPositionOld = getHandPosition(LEFT_HAND);
		}
		m_hasRightFist = false;
		rotateBasedOnHandPosition(LEFT_HAND);
	}

	else if (m_hasRightFist && !m_hasLeftFist) {
		if (m_hasRightFistReleased) {
			m_hasRightFistReleased = false;
			m_hasLeftFistReleased = true;
			handPositionOld = getHandPosition(RIGHT_HAND);
		}
		m_hasLeftFist = false;
		rotateBasedOnHandPosition(RIGHT_HAND);
	}

	else {
		m_hasRightFistReleased = m_hasLeftFistReleased = true;

		CCVector3d rotation(-thumbsticks[LEFT_HAND].x * ROTATION_SPEED, -thumbsticks[LEFT_HAND].y * ROTATION_SPEED, 1);
		rotation.normalize();

		if (rotation.x != 0.0 || rotation.y != 0.0 || rotation.z != 0.0)
		{
			m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
			m_hasRotation = true;
		}
	}

	// Assign translations
	m_translation = CCVector3(thumbsticks[RIGHT_HAND].x* TRANSLATION_SPEED, 0, thumbsticks[RIGHT_HAND].y* TRANSLATION_SPEED);
	m_hasTranslation = (m_translation.x != 0 || m_translation.z != 0);


	// Check if control applying is necessary
	if (m_hasTranslation || m_hasRotation) {
		applyControls();
	}
}

void ccOculusTouch::applyControls()
{
	if (m_hasRotation)
	{
		m_appInterface->getActiveGLWindow()->rotateBaseViewMat(m_rotation);
	}

	const ccViewportParameters& viewParams = m_appInterface->getActiveGLWindow()->getViewportParameters();

	if (m_hasTranslation) {
		float scale = static_cast<float>(std::min(m_appInterface->getActiveGLWindow()->glWidth(), m_appInterface->getActiveGLWindow()->glHeight()) * viewParams.pixelSize);
		scale /= m_appInterface->getActiveGLWindow()->computePerspectiveZoom();

		float tanFOV = tan(static_cast<float>(viewParams.fov * CC_DEG_TO_RAD));
		m_translation.x *= tanFOV;
		m_translation.y *= tanFOV;

		if (!viewParams.objectCenteredView)
		{
			scale = -scale;
		}
		m_appInterface->getActiveGLWindow()->moveCamera(-m_translation.x*scale, m_translation.y*scale, -m_translation.z*scale);
	}

	// Force GL redrawing and reset the internal controls
	m_appInterface->getActiveGLWindow()->redraw();
	resetControls();
}

void ccOculusTouch::resetControls()
{
	m_translation = CCVector3(0, 0, 0);
	m_hasTranslation = false;
	m_rotation.toIdentity();
	m_hasRotation = false;
}

ovrVector3f ccOculusTouch:: getHandPosition(unsigned int hand) {
	ovrTrackedDeviceType deviceType[1] = { hand == LEFT_HAND ? ovrTrackedDeviceType::ovrTrackedDevice_LTouch : ovrTrackedDeviceType::ovrTrackedDevice_RTouch };
	ovrPoseStatef outputPose;
	ovr_GetDevicePoses(m_ovrSession, deviceType, 1, 0, &outputPose);

	return outputPose.ThePose.Position;
}
void ccOculusTouch::rotateBasedOnHandPosition(unsigned int hand)
{
	ovrVector3f handPosition = getHandPosition(hand);

	CCVector3d rotation(
		-(handPosition.x - handPositionOld.x) * 2,
		-(handPosition.y - handPositionOld.y) * 2,
		1
	);
	rotation.normalize();
	m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
	m_hasRotation = true;
	handPositionOld = handPosition;
}
