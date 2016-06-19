#include "CinematicObjectKeyframe.h"

#include <Base/XML/XMLTreeNode.h>

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame( CXMLTreeNode &treeNode )
	: C3DElement(treeNode)
{
	m_KeyFrameTime = treeNode.GetFloatProperty( "time" );
}
