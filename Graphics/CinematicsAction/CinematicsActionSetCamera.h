#ifndef CINEMATICS_ACTION_SET_CAMERA_H
#define CINEMATICS_ACTION_SET_CAMERA_H

#include "CinematicsAction.h"

class CCinematicsActionSetCamera : public CCinematicsAction
{
public:
	std::string m_CameraName;

	CCinematicsActionSetCamera(CXMLTreeNode node);
	virtual ~CCinematicsActionSetCamera();

	void Execute();
};

#endif