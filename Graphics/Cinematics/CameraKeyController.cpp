#include "CameraKeyController.h"

#include "CameraInfo.h"
#include "CameraKey.h"
#include "Camera/Camera.h"

#include <XML/XMLTreeNode.h>


CCameraKeyController::CCameraKeyController( CXMLTreeNode &XMLTreeNode )
	: m_CurrentTime(0)
	, m_CurrentKey(0)
	, m_TotalTime(0)
	, m_Cycle(false)
	, m_Reverse(false)
{
	CXMLTreeNode key_controller = XMLTreeNode["camera_key_controller"];
	for ( int i = 0; i < key_controller.GetNumChildren(); ++i )
	{
		CXMLTreeNode keyNode = key_controller( i );
		CCameraKey *key = new CCameraKey( keyNode );
		m_Keys.push_back( key );
		if ( m_TotalTime < key->m_Time )
		{
			m_TotalTime = key->m_Time;
		}
	}

	m_Cycle = key_controller.GetBoolProperty( "cycle" );
	m_Reverse = key_controller.GetBoolProperty( "reverse" );


	if ( m_Reverse )
	{
		for ( int i = key_controller.GetNumChildren() - 1; i >= 0; --i )
		{
			CXMLTreeNode keyNode = key_controller( i );
			CCameraKey *key = new CCameraKey( keyNode );
			key->m_Time = 2 * m_TotalTime - key->m_Time;
			m_Keys.push_back( key );
		}
		m_TotalTime = m_TotalTime*2;
	}

	// TODO: should we sort the keyframes? not necessary but might be prudent
}


CCameraKeyController::~CCameraKeyController()
{
	for ( auto it = m_Keys.begin(); it != m_Keys.end(); ++it )
	{
		delete *it;
	}
	m_Keys.clear();
}


void CCameraKeyController::GetCurrentKey()
{
	if ( m_Cycle && m_CurrentTime > m_TotalTime)
	{
		while ( m_CurrentTime > m_TotalTime )
		{
			m_CurrentTime = m_CurrentTime - m_TotalTime;
		}

		m_CurrentKey = 0;
	}

	while ( m_CurrentKey < m_Keys.size()-1 && m_CurrentTime > m_Keys[m_CurrentKey + 1]->m_Time )
	{
		m_CurrentKey++;
	}
}

#define lerp(a, b, d) (a + (b-a)*d)

void CCameraKeyController::AnimateCamera(CCamera* cam) const
{
	int nextKey = m_CurrentKey + 1;


	if ( m_CurrentKey == m_Keys.size() )
	{
		nextKey = m_CurrentKey;
	}

	auto& curInf = m_Keys[m_CurrentKey]->m_CameraInfo;
	auto& nextInf = m_Keys[nextKey]->m_CameraInfo;

	float t = ( m_CurrentTime - m_Keys[m_CurrentKey]->m_Time );

	Vect3f pos = lerp(curInf.m_Eye, nextInf.m_Eye, t);
	Vect3f up = lerp(curInf.m_Up, nextInf.m_Up, t);
	Vect3f lookat = lerp(curInf.m_LookAt, nextInf.m_LookAt, t);
	float near = lerp(curInf.m_NearPlane, nextInf.m_NearPlane, t);
	float far = lerp(curInf.m_FarPlane, nextInf.m_FarPlane, t);
	float fov = lerp(curInf.m_FOV, nextInf.m_FOV, t);

	cam->SetPosition( pos );
	cam->SetLookAt( lookat );
	cam->SetUp( up );
	cam->SetZNear( near );
	cam->SetZFar( far );
	cam->SetFOV( fov );
}


void CCameraKeyController::Update( float ElapsedTime )
{
	m_CurrentTime += ElapsedTime;
	GetCurrentKey();
}


void CCameraKeyController::SetCurrentTime( float CurrentTime )
{
	m_CurrentTime = CurrentTime;
	GetCurrentKey();
}


void CCameraKeyController::ResetTime()
{
	m_CurrentTime = 0;
	m_CurrentKey = 0;
}


float CCameraKeyController::GetTotalTime()
{
	return m_TotalTime;
}
