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

#include <QMenu>

#include "ccOculusTouch.h"
#include "ccGLWindow.h"

#include "ccMainAppInterface.h"
#include <algorithm>


void ccOculusTouch::update()
{
	double actualTime = ovr_GetTimeInSeconds();
	double deltaTime = actualTime - m_timeStamp;
	m_timeStamp = actualTime;

	m_viewParameters = m_appInterface->getActiveGLWindow()->getViewportParameters();

	ovrInputState inputState;
	ovr_GetInputState(m_ovrSession, m_controllerType, &inputState);

	updateGestures(inputState);
	updateThumbSticks(inputState, deltaTime);
	updateButtons(inputState);

	if (m_hasTranslation || m_hasRotation) {
		applyControls();
	}

	if (m_shouldRedraw) {
		m_appInterface->getActiveGLWindow()->redraw();
		resetControls();
	}
}

void ccOculusTouch::applyControls()
{
	if (m_hasRotation)
	{
		m_appInterface->getActiveGLWindow()->rotateBaseViewMat(m_rotation);
	}

	if (m_hasTranslation) {
		float scale = static_cast<float>(std::min(m_appInterface->getActiveGLWindow()->glWidth(), m_appInterface->getActiveGLWindow()->glHeight()) * m_viewParameters.pixelSize);
		scale /= m_appInterface->getActiveGLWindow()->computePerspectiveZoom();

		const float tanFOV = tan(static_cast<float>(m_viewParameters.fov * CC_DEG_TO_RAD)/*/2*/);
		m_translation.x *= tanFOV;
		m_translation.y *= tanFOV;

		if (!m_viewParameters.objectCenteredView)
		{
			scale = -scale;
		}
		m_appInterface->getActiveGLWindow()->moveCamera(-m_translation.x*scale, m_translation.y*scale, -m_translation.z*scale);
	}

	m_shouldRedraw = true;
}

void ccOculusTouch::resetControls()
{
	m_translation = CCVector3(0, 0, 0);
	m_rotation.toIdentity();

	m_hasTranslation = false;
	m_hasRotation = false;
	m_shouldRedraw = false;
}

void ccOculusTouch::updateButtons(const ovrInputState& inputState)
{
	const unsigned int buttonState = inputState.Buttons;

	const bool buttonA = buttonState & ovrButton_A;
	const bool buttonB = buttonState & ovrButton_B;
	const bool buttonX = buttonState & ovrButton_X;
	const bool buttonY = buttonState & ovrButton_Y;

	if (buttonA && !m_buttonAPressed) {
		m_appInterface->decreasePointSize();
	}
	if (buttonB && !m_buttonBPressed) {
		m_appInterface->increasePointSize();
	}
	if (buttonX && !m_buttonXPressed) {
		//m_appInterface->getActiveGLWindow()->setPerspectiveState(true, !m_viewParameters.objectCenteredView);
	}
	if (buttonY && !m_buttonYPressed) {
		m_appInterface->getActiveGLWindow()->zoomGlobal();
	}

	m_buttonAPressed = buttonA;
	m_buttonBPressed = buttonB;
	m_buttonXPressed = buttonX;
	m_buttonYPressed = buttonY;
}

void ccOculusTouch::updateGestures(const ovrInputState& inputState)
{
	const bool leftFist = (inputState.HandTrigger[c_leftHand] >= c_triggerDeadZone && inputState.IndexTrigger[c_leftHand] >= c_triggerDeadZone);
	const bool rightFist = (inputState.HandTrigger[c_rightHand] >= c_triggerDeadZone && inputState.IndexTrigger[c_rightHand] >= c_triggerDeadZone);

	// First update the inital old position of a hand gesture
	if (leftFist && !m_hasLeftFist) {
		m_fistPositionOld[c_leftHand] = getHandPosition(c_leftHand);
	}
	if (rightFist && !m_hasRightFist) {
		m_fistPositionOld[c_rightHand] = getHandPosition(c_rightHand);
	}

	// Then decide which gesture was performed
	if (leftFist && rightFist) {
		PreCalculateDoubleHandGestures();
	}

	else if (leftFist && !rightFist) {
		PreCalculateTranslationBasedOnHandPosition(c_leftHand);
	}

	else if (rightFist && !leftFist) {
		PreCalculateXYRotationBasedOnHandPosition(c_rightHand);
	}

	m_hasLeftFist = leftFist;
	m_hasRightFist = rightFist;
}

void ccOculusTouch::updateThumbSticks(const ovrInputState & inputState, const double& deltaTime)
{
	const ovrVector2f *thumbsticks = inputState.Thumbstick;
	PreCalculateRotationBasedOnThumbStick(thumbsticks, deltaTime);
	PreCalculateTranslationBasedOnThumbStick(thumbsticks, deltaTime);
}

ovrVector3f ccOculusTouch:: getHandPosition(unsigned int hand) {
	ovrTrackedDeviceType deviceType[1] = { hand == c_leftHand ? ovrTrackedDeviceType::ovrTrackedDevice_LTouch : ovrTrackedDeviceType::ovrTrackedDevice_RTouch };
	ovrPoseStatef outputPose;
	ovr_GetDevicePoses(m_ovrSession, deviceType, 1, 0, &outputPose);

	return outputPose.ThePose.Position;
}
void ccOculusTouch::PreCalculateDoubleHandGestures()
{
	// Rotation around Y and Z axis
	const ovrVector3f handPosLeft = getHandPosition(c_leftHand);
	const ovrVector3f handPosRight = getHandPosition(c_rightHand);

	const float currentDistanceX = handPosRight.x - handPosLeft.x;
	const float oldDistanceX = m_fistPositionOld[c_rightHand].x - m_fistPositionOld[c_leftHand].x;

	const double zRotationAngle = getAngleBetween2dVectors(
		currentDistanceX, handPosRight.y - handPosLeft.y,
		oldDistanceX, m_fistPositionOld[c_rightHand].y - m_fistPositionOld[c_leftHand].y,
		false
	);

	const double yRotationAngle = getAngleBetween2dVectors(
		currentDistanceX, handPosRight.z - handPosLeft.z,
		oldDistanceX, m_fistPositionOld[c_rightHand].z - m_fistPositionOld[c_leftHand].z,
		true
	);

	ccGLMatrixd rot;
	rot.initFromParameters(zRotationAngle, CCVector3d(0, 0, 1), CCVector3d(0, 0, 0));
	m_rotation = rot * m_rotation;
	rot.initFromParameters(yRotationAngle, CCVector3d(0, 1, 0), CCVector3d(0, 0, 0));
	m_rotation = rot * m_rotation;

	m_hasRotation = true;

	// Translation along X and Y axis
	const float deltaXLeftNew = handPosLeft.x - m_fistPositionOld[c_leftHand].x;
	const float deltaXRightNew = handPosRight.x - m_fistPositionOld[c_rightHand].x;
	const float deltaYLeftNew = handPosLeft.y - m_fistPositionOld[c_leftHand].y;
	const float deltaYRightNew = handPosRight.y - m_fistPositionOld[c_rightHand].y;
	const float deltaZLeftNew = handPosLeft.z - m_fistPositionOld[c_leftHand].z;
	const float deltaZRightNew = handPosRight.z - m_fistPositionOld[c_rightHand].z;

	const float hasXTranslation = (deltaXLeftNew > 0 && deltaXRightNew > 0) || (deltaXLeftNew < 0 && deltaXRightNew < 0);
	const float hasYTranslation = (deltaYLeftNew > 0 && deltaYRightNew > 0) || (deltaYLeftNew < 0 && deltaYRightNew < 0);
	const float hasZTranslation = (deltaZLeftNew > 0 && deltaZRightNew > 0) || (deltaZLeftNew < 0 && deltaZRightNew < 0);

	if (hasXTranslation || hasYTranslation || hasZTranslation) {
		float xTranslation = hasXTranslation ? deltaXLeftNew : 0;
		float yTranslation = hasYTranslation ? deltaYLeftNew : 0;
		float zTranslation = hasZTranslation ? deltaZLeftNew : 0;

		m_translation = CCVector3(
			xTranslation * m_translationSpeed * 2,
			yTranslation * m_translationSpeed * 2 * (-1),
			zTranslation * m_translationSpeed * 2
		);

		m_hasTranslation = true;
	}

	m_fistPositionOld[c_leftHand] = handPosLeft;
	m_fistPositionOld[c_rightHand] = handPosRight;
}
void ccOculusTouch::PreCalculateXYRotationBasedOnHandPosition(const unsigned int& hand)
{
	const ovrVector3f handPosition = getHandPosition(hand);

	CCVector3d rotation(
		-(handPosition.x - m_fistPositionOld[hand].x) * 2,
		-(handPosition.y - m_fistPositionOld[hand].y) * 2,
		1
	);

	if (m_viewParameters.objectCenteredView) {
		rotation.x = rotation.x * -1;
		rotation.y = rotation.y * -1;
	}

	rotation.normalize();
	m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
	m_hasRotation = true;
	m_fistPositionOld[hand] = handPosition;
}

void ccOculusTouch::PreCalculateTranslationBasedOnHandPosition(const unsigned int& hand)
{
	const ovrVector3f handPosition = getHandPosition(hand);

	m_translation = CCVector3(
		(handPosition.x - m_fistPositionOld[hand].x) * m_translationSpeed * 2,
		(handPosition.y - m_fistPositionOld[hand].y) * m_translationSpeed * 2 * -1,
		(handPosition.z - m_fistPositionOld[hand].z) * m_translationSpeed * 2
	);
	m_fistPositionOld[hand] = handPosition;

	m_hasTranslation = true;
}

void ccOculusTouch::PreCalculateRotationBasedOnThumbStick(const ovrVector2f *thumbsticks, const double& deltaTime)
{
	if (thumbsticks[c_rightHand].x != 0 || thumbsticks[c_rightHand].y) {
		CCVector3d rotation(
			thumbsticks[c_rightHand].x * m_rotationSpeed * deltaTime,
			thumbsticks[c_rightHand].y * m_rotationSpeed * deltaTime,
			1
		);
		rotation.normalize();

		m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
		m_hasRotation = true;
	}
}

void ccOculusTouch::PreCalculateTranslationBasedOnThumbStick(const ovrVector2f * thumbsticks, const double& deltaTime)
{
	if (thumbsticks[c_leftHand].x != 0 || thumbsticks[c_leftHand].y != 0) {
		m_translation = CCVector3(
			thumbsticks[c_leftHand].x * m_translationSpeed * deltaTime,
			0,
			thumbsticks[c_leftHand].y * m_translationSpeed * deltaTime
		);

		m_hasTranslation = true;
	}
}

double ccOculusTouch::getAngleBetween2dVectors(const float& aVec1, const float& bVec1, const float& aVec2, const float& bVec2, const bool& clockWiseRotation) const
{
	CCVector2 vector1(aVec1, bVec1);
	CCVector2 vector2(aVec2, bVec2);
	vector1.normalize();
	vector2.normalize();

	PointCoordinateType rotationDir = vector1.cross(vector2);

	int sign = clockWiseRotation ? 1 : -1;
	double angle = static_cast<double>(acos(vector1.dot(vector2))) * ((rotationDir > 0) ? sign : (sign * (-1)));
	if (angle != angle) {
		angle = 0;
	}
	return angle;
}
