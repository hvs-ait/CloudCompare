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

#ifndef CC_OCULUS_CONTROLLER
#define CC_OCULUS_CONTROLLER

#include <QObject>
#include "oculus/ccOculus.h"

class QAction;
class QMenu;

class ccMainAppInterface;


class ccOculusController : public QObject
{
	Q_OBJECT

public:
	ccOculusController(ccMainAppInterface *appInterface, ovrSession ovrSession):
		m_appInterface(appInterface),
		m_ovrSession(ovrSession) 
	{}
	virtual ~ccOculusController() = default;
	virtual void initController() = 0;
	virtual void update() = 0;

protected:
	virtual void resetControls() = 0;
	virtual void applyControls() = 0;

protected:
	ccMainAppInterface *m_appInterface;
    ovrSession m_ovrSession;
	ovrControllerType m_controllerType;
};

#endif
