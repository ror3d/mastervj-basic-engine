#include "CameraInfo.h"

#include "XML/XMLTreeNode.h"

#include <Math/Math.h>

#include <sstream>
#include <Math/MathUtils.h>

CCameraInfo::CCameraInfo()
	: m_FOV(mathUtils::Deg2Rad(70.0f))
	, m_AspectRatio(1.0f)
	, m_ZNear(0.1f)
	, m_ZFar(1000.0f)
	, m_Position(0.0f, 0.0f, 0.0f)
	, m_LookAt(0.0f, 0.0f, 1.0f)
	, m_Up(0.0f, 1.0f, 0.0f)
{
}


CCameraInfo::CCameraInfo( const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float
	NearPlane, float FarPlane, float FOV, float AspectRatio )
	: m_Position(Eye)
	, m_LookAt(LookAt)
	, m_Up(Up)
	, m_ZNear(NearPlane)
	, m_ZFar(FarPlane)
	, m_FOV(FOV)
	, m_AspectRatio(AspectRatio)
{
}


CCameraInfo::CCameraInfo( CXMLTreeNode &node )
{
	std::stringstream ss;
	ss.str( node.GetPszProperty( "pos", "0 0 0") );
	ss >> m_Position;
	ss.clear();
	ss.str( node.GetPszProperty( "look_at", "0 0 1") );
	ss >> m_LookAt;
	ss.clear();
	ss.str( node.GetPszProperty( "up_vector", "0 1 0") );
	ss >> m_Up;
	ss.clear();
	ss.str( node.GetPszProperty( "fov", "90") );
	ss >> m_FOV;
	m_FOV = mathUtils::Deg2Rad( m_FOV );
	ss.clear();
	ss.str( node.GetPszProperty( "near_plane", "0") );
	ss >> m_ZNear;
	ss.clear();
	ss.str( node.GetPszProperty( "far_plane", "1000") );
	ss >> m_ZFar;
	ss.clear();
}
