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

#ifndef CC_OCULUS_CONTROLLER_MANAGER
#define CC_OCULUS_CONTROLLER_MANAGER

// std
#include <memory>

// Qt
#include <QObject>

// CC
#include "ccOculusController.h"

class ccOculusControllerManager : public QObject
{
	Q_OBJECT

public:
	explicit ccOculusControllerManager( ccMainAppInterface *appInterface, QObject *parent );
	bool initializeController(ovrSession ovr, const ovrControllerType& controllerType, float rotationSpeed, float translationSpeed);
	void disableController();

private:
	ccMainAppInterface *m_appInterface;
	ovrSession m_ovrSession;
	std::unique_ptr<ccOculusController> m_controller;
};

#endif
