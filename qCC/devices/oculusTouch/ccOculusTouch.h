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

#ifndef CC_OOCULUS_TOUCH
#define CC_OOCULUS_TOUCH

#include <QObject>
#include "oculus/ccOculus.h"

class QAction;
class QMenu;

class ccMainAppInterface;


class ccOculusTouch : public QObject
{
	Q_OBJECT

public:
	ccOculusTouch( ccMainAppInterface *appInterface, QObject *parent );
	~ccOculusTouch();

    
	ccMainAppInterface *m_appInterface;
    ovrSession m_ovrSession;
};

#endif
