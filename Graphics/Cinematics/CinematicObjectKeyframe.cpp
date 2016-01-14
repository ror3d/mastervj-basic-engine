#include "CinematicObjectKeyframe.h"


CCinematicObjectKeyFrame::CCinematicObjectKeyFrame( CXMLTreeNode &treeNode )
	: C3DElement(treeNode)
{
	m_KeyFrameTime = treeNode.GetFloatProperty( "time" );
}
