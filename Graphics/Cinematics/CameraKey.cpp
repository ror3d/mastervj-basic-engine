#include "CameraKey.h"

#include <XML/XMLTreeNode.h>

CCameraKey::CCameraKey( CXMLTreeNode &treeNode )
	: m_CameraInfo(treeNode)
{
	m_Time = treeNode.GetFloatProperty( "time" );
}

CCameraKey::CCameraKey( CCameraInfo &CameraInfo, float Time )
	: m_CameraInfo(CameraInfo)
	, m_Time(Time)
{
}
