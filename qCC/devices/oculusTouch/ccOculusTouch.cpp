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

	if (redraw) {
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

		float tanFOV = tan(static_cast<float>(m_viewParameters.fov * CC_DEG_TO_RAD)/*/2*/);
		m_translation.x *= tanFOV;
		m_translation.y *= tanFOV;

		if (!m_viewParameters.objectCenteredView)
		{
			scale = -scale;
		}
		m_appInterface->getActiveGLWindow()->moveCamera(-m_translation.x*scale, m_translation.y*scale, -m_translation.z*scale);
	}

	redraw = true;
}

void ccOculusTouch::resetControls()
{
	m_translation = CCVector3(0, 0, 0);
	m_hasTranslation = false;
	m_rotation.toIdentity();
	m_hasRotation = false;
	redraw = false;
}

void ccOculusTouch::updateButtons(const ovrInputState& inputState)
{
	const unsigned int buttonState = inputState.Buttons;

	bool buttonA = buttonState & ovrButton_A;
	bool buttonB = buttonState & ovrButton_B;
	bool buttonX = buttonState & ovrButton_X;
	bool buttonY = buttonState & ovrButton_Y;

	if (buttonA && !m_buttonA) {
		m_appInterface->decreasePointSize();
	}
	if (buttonB && !m_buttonB) {
		m_appInterface->increasePointSize();
	}
	if (buttonX && !m_buttonX) {
		//m_appInterface->getActiveGLWindow()->setPerspectiveState(true, !m_viewParameters.objectCenteredView);
	}
	if (buttonY && !m_buttonY) {
		m_appInterface->getActiveGLWindow()->zoomGlobal();
	}

	m_buttonA = buttonA;
	m_buttonB = buttonB;
	m_buttonX = buttonX;
	m_buttonY = buttonY;
}

void ccOculusTouch::updateGestures(const ovrInputState& inputState)
{
	bool leftFist = (inputState.HandTrigger[c_left_hand] >= 0.8 && inputState.IndexTrigger[c_left_hand] >= 0.8);
	bool rightFist = (inputState.HandTrigger[c_right_hand] >= 0.8 && inputState.IndexTrigger[c_right_hand] >= 0.8);

	// First update the inital old position of a hand gesture
	if (leftFist && !m_hasLeftFist) {
		m_handPositionOld[c_left_hand] = getHandPosition(c_left_hand);
	}
	if (rightFist && !m_hasRightFist) {
		m_handPositionOld[c_right_hand] = getHandPosition(c_right_hand);
	}

	// Then decide which gesture was performed
	if (leftFist && rightFist) {
		PreCalculateZRotationBasedOnHandPosition();
	}

	else if (leftFist && !rightFist) {
		PreCalculateTranslationBasedOnHandPosition(c_left_hand);
	}

	else if (rightFist && !leftFist) {
		PreCalculateXYRotationBasedOnHandPosition(c_right_hand);
	}

	m_hasLeftFist = leftFist;
	m_hasRightFist = rightFist;
}

void ccOculusTouch::updateThumbSticks(const ovrInputState & inputState, double deltaTime)
{
	const ovrVector2f *thumbsticks = inputState.Thumbstick;
	PreCalculateRotationBasedOnThumbStick(thumbsticks, deltaTime);
	PreCalculateTranslationBasedOnThumbStick(thumbsticks, deltaTime);
}

ovrVector3f ccOculusTouch:: getHandPosition(unsigned int hand) {
	ovrTrackedDeviceType deviceType[1] = { hand == c_left_hand ? ovrTrackedDeviceType::ovrTrackedDevice_LTouch : ovrTrackedDeviceType::ovrTrackedDevice_RTouch };
	ovrPoseStatef outputPose;
	ovr_GetDevicePoses(m_ovrSession, deviceType, 1, 0, &outputPose);

	return outputPose.ThePose.Position;
}
void ccOculusTouch::PreCalculateZRotationBasedOnHandPosition()
{

	ovrVector3f handPosLeft = getHandPosition(c_left_hand);
	ovrVector3f handPosRight = getHandPosition(c_right_hand);

	// Z - Rotation
	CCVector2 spanningVectorNew(handPosRight.x - handPosLeft.x, handPosRight.y - handPosLeft.y);
	CCVector2 spanningVectorOld(m_handPositionOld[c_right_hand].x - m_handPositionOld[c_left_hand].x, m_handPositionOld[c_right_hand].y - m_handPositionOld[c_left_hand].y);
	spanningVectorNew.normalize();
	spanningVectorOld.normalize();

	PointCoordinateType rotationDir = spanningVectorNew.cross(spanningVectorOld);

	double angle = static_cast<double>(acos(spanningVectorNew.dot(spanningVectorOld))) * ((rotationDir > 0) ? -1 : 1);
	if (angle != angle) {
		angle = 0;
	}

	if (angle != 0) {

		ccGLMatrixd rot;
		rot.initFromParameters(angle, CCVector3d(0, 0, 1), CCVector3d(0, 0, 0));
		m_rotation = rot * m_rotation;

		m_hasRotation = true;
	}

	// Y Rotation
	spanningVectorNew = CCVector2(handPosRight.x - handPosLeft.x, handPosRight.z - handPosLeft.z);
	spanningVectorOld = CCVector2(m_handPositionOld[c_right_hand].x - m_handPositionOld[c_left_hand].x, m_handPositionOld[c_right_hand].z - m_handPositionOld[c_left_hand].z);
	spanningVectorNew.normalize();
	spanningVectorOld.normalize();

	rotationDir = spanningVectorNew.cross(spanningVectorOld);

	angle = static_cast<double>(acos(spanningVectorNew.dot(spanningVectorOld))) * ((rotationDir > 0) ? 1 : -1);
	if (angle != angle) {
		angle = 0;
	}

	if (angle != 0) {

		ccGLMatrixd rot;
		rot.initFromParameters(angle, CCVector3d(0, 1, 0), CCVector3d(0, 0, 0));
		m_rotation = rot * m_rotation;

		m_hasRotation = true;
	}

	// XY - Translation
	float deltaXLeftNew = handPosLeft.x - m_handPositionOld[c_left_hand].x;
	float deltaXRightNew = handPosRight.x - m_handPositionOld[c_right_hand].x;
	float deltaYLeftNew = handPosLeft.y - m_handPositionOld[c_left_hand].y;
	float deltaYRightNew = handPosRight.y - m_handPositionOld[c_right_hand].y;

	float hasXTranslation = (deltaXLeftNew > 0 && deltaXRightNew > 0) || (deltaXLeftNew < 0 && deltaXRightNew < 0);
	float hasYTranslation = (deltaYLeftNew > 0 && deltaYRightNew > 0) || (deltaYLeftNew < 0 && deltaYRightNew < 0);

	if (hasXTranslation || hasYTranslation) {
		float xTranslation = hasXTranslation ? deltaXLeftNew : 0;
		float yTranslation = hasYTranslation ? deltaYLeftNew : 0;

		m_translation = CCVector3(
			xTranslation * m_translationSpeed * 2,
			yTranslation * m_translationSpeed * 2 * (-1),
			0
		);

		m_hasTranslation = true;
	}

	m_handPositionOld[c_left_hand] = handPosLeft;
	m_handPositionOld[c_right_hand] = handPosRight;
}
void ccOculusTouch::PreCalculateXYRotationBasedOnHandPosition(unsigned int hand)
{
	ovrVector3f handPosition = getHandPosition(hand);

	CCVector3d rotation(
		-(handPosition.x - m_handPositionOld[hand].x) * 2,
		-(handPosition.y - m_handPositionOld[hand].y) * 2,
		1
	);

	if (m_viewParameters.objectCenteredView) {
		rotation.x = rotation.x * -1;
		rotation.y = rotation.y * -1;
	}

	rotation.normalize();
	m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
	m_hasRotation = true;
	m_handPositionOld[hand] = handPosition;
}

void ccOculusTouch::PreCalculateTranslationBasedOnHandPosition(unsigned int hand)
{
	ovrVector3f handPosition = getHandPosition(hand);

	m_translation = CCVector3(
		(handPosition.x - m_handPositionOld[hand].x) * m_translationSpeed * 2,
		(handPosition.y - m_handPositionOld[hand].y) * m_translationSpeed * 2 * -1,
		(handPosition.z - m_handPositionOld[hand].z) * m_translationSpeed * 2
	);
	m_handPositionOld[hand] = handPosition;

	m_hasTranslation = true;
}

void ccOculusTouch::PreCalculateRotationBasedOnThumbStick(const ovrVector2f *thumbsticks, double deltaTime)
{
	if (thumbsticks[c_right_hand].x != 0 || thumbsticks[c_right_hand].y) {
		CCVector3d rotation(
			thumbsticks[c_right_hand].x * m_rotationSpeed * deltaTime,
			thumbsticks[c_right_hand].y * m_rotationSpeed * deltaTime,
			1
		);
		rotation.normalize();

		m_rotation = ccGLMatrixd::FromToRotation(CCVector3d(0, 0, 1), rotation);
		m_hasRotation = true;
	}
}

void ccOculusTouch::PreCalculateTranslationBasedOnThumbStick(const ovrVector2f * thumbsticks, double deltaTime)
{
	if (thumbsticks[c_left_hand].x != 0 || thumbsticks[c_left_hand].y != 0) {
		m_translation = CCVector3(
			thumbsticks[c_left_hand].x * m_translationSpeed * deltaTime,
			0,
			thumbsticks[c_left_hand].y * m_translationSpeed * deltaTime
		);

		m_hasTranslation = true;
	}
}
