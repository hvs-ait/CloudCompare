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

#ifndef CC_OCULUS_CONTROLLER_MANAGER
#define CC_OCULUS_CONTROLLER_MANAGER

#include <QObject>
#include "oculus/ccOculus.h"

class QAction;
class QMenu;

class ccMainAppInterface;


class ccOculusControllerManager : public QObject
{
	Q_OBJECT

public:
	ccOculusControllerManager( ccMainAppInterface *appInterface, QObject *parent );
	~ccOculusControllerManager();

	bool setSession(ovrSession ovr, ovrControllerType controllerType);

public slots:
	void onUpdateRequest();

private:
	void enableDevice(bool state, bool silent);
	void releaseDevice();
    
	ccMainAppInterface *m_appInterface;

	QAction *m_actionEnable;

	ovrSession m_ovrSession;
};

#endif
