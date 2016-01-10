#include "CameraInfo.h"

#include "XML/XMLTreeNode.h"

#include <sstream>

CCameraInfo::CCameraInfo()
	: m_Eye(0, 0, 0)
	, m_LookAt(0, 0, 1)
	, m_Up(0, 1, 0)
	, m_FOV(90)
	, m_NearPlane(0)
	, m_FarPlane(1000)
{
}


CCameraInfo::CCameraInfo( const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float
	NearPlane, float FarPlane, float FOV )
	: m_Eye(Eye)
	, m_LookAt(LookAt)
	, m_Up(Up)
	, m_NearPlane(NearPlane)
	, m_FarPlane(FarPlane)
	, m_FOV(FOV)
{
}


CCameraInfo::CCameraInfo( CXMLTreeNode &node )
{
	std::stringstream ss;
	ss.str( node.GetPszProperty( "pos", "0 0 0") );
	ss >> m_Eye;
	ss.str( node.GetPszProperty( "look_at", "0 0 1") );
	ss >> m_LookAt;
	ss.str( node.GetPszProperty( "up_vector", "0 1 0") );
	ss >> m_Up;
	ss.str( node.GetPszProperty( "fov", "90") );
	ss >> m_FOV;
	ss.str( node.GetPszProperty( "near_plane", "0") );
	ss >> m_NearPlane;
	ss.str( node.GetPszProperty( "far_plane", "1000") );
	ss >> m_FarPlane;
}
