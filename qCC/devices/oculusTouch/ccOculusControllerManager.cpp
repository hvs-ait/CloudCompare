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
//#          COPYRIGHT: CloudCompare project                               #
//#                                                                        #
//##########################################################################

#include <QAction>
#include <QMainWindow>
#include <QMenu>

#include "ccOculusControllerManager.h"
#include "ccGLWindow.h"

#include "ccMainAppInterface.h"


ccOculusControllerManager::ccOculusControllerManager( ccMainAppInterface *appInterface, QObject *parent ) :
    QObject( parent ),
    m_appInterface( appInterface )
{
   
}

ccOculusControllerManager::~ccOculusControllerManager()
{
    releaseDevice();
}

bool ccOculusControllerManager::setSession(ovrSession ovr, ovrControllerType controllerType)
{
	m_ovrSession = ovr;
	unsigned connectedControllerType = ovr_GetConnectedControllerTypes(m_ovrSession);
	if (controllerType == 0) {
		ccLog::Warning("No Oculus Controller connected!");
		return false;
	}
	if (controllerType != connectedControllerType) {
		ccLog::Warning("Connected controller does not match selected controller type!");
		return false;
	}
	if (connectedControllerType == ovrControllerType::ovrControllerType_Touch) {
		ccLog::Print("Oculus Controller connected!");
		return true;
	}
	else {
		ccLog::Error("Controller type not supported!");
		return false;
	}
}

void ccOculusControllerManager::onUpdateRequest()
{
	ccLog::Print("Update");
}

void ccOculusControllerManager::enableDevice(bool state, bool silent)
{
	ccLog::Print("Oculus Touch controllers initialized");
}

void ccOculusControllerManager::releaseDevice()
{
	
}
