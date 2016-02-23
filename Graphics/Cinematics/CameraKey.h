#pragma once

#include "Camera/CameraInfo.h"

class CXMLTreeNode;

class CCameraKey
{
public:
	CCameraInfo m_CameraInfo;
	float m_Time;
	CCameraKey( CXMLTreeNode &keyNode );
	CCameraKey( CCameraInfo &CameraInfo, float Time );
};
