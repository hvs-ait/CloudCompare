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

// Qt
#include <qsize.h>

// Oculus OVR SDK
#include "ccOculusTouch.h"

// CC
#include "ccOculusControllerManager.h"


ccOculusControllerManager::ccOculusControllerManager( ccMainAppInterface *appInterface, QObject *parent ) :
    QObject( parent ),
    m_appInterface( appInterface ),
	m_controller( nullptr )
{}

bool ccOculusControllerManager::initializeController(ovrSession ovr, const ovrControllerType& controllerType, float rotationSpeed, float translationSpeed)
{
	m_ovrSession = ovr;
	unsigned connectedControllerType = ovr_GetConnectedControllerTypes(m_ovrSession);
	if (controllerType == 0x0000) {
		ccLog::Warning("[OculusController] No Oculus Controller connected!");
		return false;
	}
	if (controllerType != connectedControllerType) {
		ccLog::Warning("[OculusController] Could not find matching controller!");
		return false;
	}
	if (connectedControllerType == ovrControllerType::ovrControllerType_Touch) {
		m_controller = std::make_unique<ccOculusTouch>(m_appInterface, m_ovrSession, rotationSpeed, translationSpeed);
		return true;
	}
	else {
		ccLog::Error("[OculusController] Controller type is not supported!");
		return false;
	}
}

void ccOculusControllerManager::onUpdateRequest()
{
	if (m_controller) {
		m_controller->update();
	}
}
