#include "CameraKeyController.h"

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

void CCameraKeyController::UpdateCameraValues(CCamera* cam) const
{
	int nextKey = m_CurrentKey + 1;


	if ( nextKey == m_Keys.size() )
	{
		nextKey = m_CurrentKey;
	}

	auto& curInf = m_Keys[m_CurrentKey]->m_CameraInfo;
	auto& nextInf = m_Keys[nextKey]->m_CameraInfo;

	float t = 1;
	if ( nextKey != m_CurrentKey )
	{
		t = ( m_CurrentTime - m_Keys[m_CurrentKey]->m_Time ) / ( m_Keys[nextKey]->m_Time - m_Keys[m_CurrentKey]->m_Time );
	}

	Vect3f pos = mathUtils::Lerp(curInf.GetPosition(), nextInf.GetPosition(), t);
	Vect3f up = mathUtils::Lerp(curInf.GetUp(), nextInf.GetUp(), t);
	Vect3f lookat = mathUtils::Lerp(curInf.GetLookAt(), nextInf.GetLookAt(), t);
	float near = mathUtils::Max(0.0001f, mathUtils::Lerp(curInf.GetZNear(), nextInf.GetZNear(), t));
	float far = mathUtils::Lerp(curInf.GetZFar(), nextInf.GetZFar(), t);
	float fov = mathUtils::Lerp(curInf.GetFOV(), nextInf.GetFOV(), t);

	cam->SetPosition( pos );
	cam->SetLookAt( lookat + pos );
	cam->SetUp( up );
	cam->SetZNear( near );
	cam->SetZFar( far );
	cam->SetFOV( fov );
	cam->SetMatrixs();
}


void CCameraKeyController::Update( float ElapsedTime )
{
	m_CurrentTime += ElapsedTime;
	GetCurrentKey();
}


void CCameraKeyController::SetCurrentTime( float CurrentTime )
{
	m_CurrentTime = CurrentTime;
	m_CurrentKey = 0;
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
