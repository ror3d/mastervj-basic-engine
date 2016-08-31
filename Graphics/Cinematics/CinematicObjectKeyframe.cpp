#include "CinematicObjectKeyframe.h"

#include <Base/XML/XMLTreeNode.h>

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame( CXMLTreeNode &node )
{
	m_KeyFrameTime = node.GetFloatProperty( "time" );

	m_Position = node.GetVect3fProperty( "pos" , Vect3f(0, 0, 0), false);
	m_Scale = node.GetVect3fProperty( "scale" , Vect3f(1, 1, 1), false);
	if (node.GetPszProperty("rotationq", nullptr, false) != nullptr)
	{
		Vect4f f4 = node.GetVect4fProperty("rotationq", Vect4f(0, 0, 0, 1), true);
		m_quat = Quatf(f4.x, f4.y, f4.z, f4.w);

	}
	else
	{
		float yaw, pitch, roll;
		if (node.GetPszProperty("rotation", nullptr, false) != nullptr)
		{
			Vect3f rot = node.GetVect3fProperty("rotation", Vect3f(0, 0, 0), false);
			yaw = rot.x;
			pitch = rot.y;
			roll = rot.z;
		}
		else
		{
			yaw = node.GetFloatProperty("yaw");
			pitch = node.GetFloatProperty("pitch");
			roll = node.GetFloatProperty("roll");
		}
		Vect3f RotationYPR;
		RotationYPR.x = mathUtils::Deg2Rad(yaw);
		RotationYPR.y = mathUtils::Deg2Rad(pitch);
		RotationYPR.z = mathUtils::Deg2Rad(roll);
		m_quat = Quatf::GetQuaternionFromRadians( RotationYPR );
	}

}
