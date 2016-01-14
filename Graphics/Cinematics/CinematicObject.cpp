#include "CinematicObject.h"

#include "CinematicObjectKeyFrame.h"

#include "Core/Engine/Engine.h"
#include "Renderable/RenderableObjectsManager.h"
#include "Renderable/RenderableObject.h"

#include <XML/XMLTreeNode.h>

CCinematicObject::CCinematicObject( CXMLTreeNode &treeNode )
	: m_CurrentKeyFrame(0)
{
	std::string rendObjName = treeNode.GetPszProperty( "resource" );
	m_RenderableObject = CEngine::GetSingleton().getRenderableObjectManager()->get(rendObjName);


	float duration = 0;
	for ( int i = 0; i < treeNode.GetNumChildren(); ++i )
	{
		auto kf = new CCinematicObjectKeyFrame( treeNode( i ) );
		AddCinematicObjectKeyFrame(kf);
		if ( kf->getKeyFrameTime() > duration )
		{
			duration = kf->getKeyFrameTime();
		}
	}

	CCinematicPlayer::Init( duration );
}


CCinematicObject::~CCinematicObject()
{
	for ( auto it = m_CinematicObjectKeyFrames.begin(); it != m_CinematicObjectKeyFrames.end(); ++it )
	{
		delete ( *it );
	}
}


bool CCinematicObject::IsOk()
{
	// TODO
	return true;
}


void CCinematicObject::AddCinematicObjectKeyFrame( CCinematicObjectKeyFrame *CinematicObjectKeyFrame )
{
	m_CinematicObjectKeyFrames.push_back( CinematicObjectKeyFrame );
}


void CCinematicObject::Update( float ElapsedTime )
{
	CCinematicPlayer::Update( ElapsedTime );
	
	m_CurrentKeyFrame = 0;
	while (m_CurrentKeyFrame < m_CinematicObjectKeyFrames.size() - 2
		   && m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->getKeyFrameTime() < m_CurrentTime)
	{
		m_CurrentKeyFrame++;
	}

	int nextKF = mathUtils::Min( m_CurrentKeyFrame + 1, m_CinematicObjectKeyFrames.size()-1);

	auto current = m_CinematicObjectKeyFrames[m_CurrentKeyFrame];
	auto next = m_CinematicObjectKeyFrames[nextKF];

	float t = 0;
	if ( nextKF != m_CurrentKeyFrame )
	{
		t = ( getCurrentTime() - current->getKeyFrameTime() ) / ( next->getKeyFrameTime() - current->getKeyFrameTime() );
	}

	m_RenderableObject->SetPosition(mathUtils::Lerp(current->GetPosition(), next->GetPosition(), t));
	m_RenderableObject->SetYawPitchRoll(
		mathUtils::Lerp( current->GetYaw(), next->GetYaw(), t ),
		mathUtils::Lerp( current->GetPitch(), next->GetPitch(), t ),
		mathUtils::Lerp( current->GetRoll(), next->GetRoll(), t ) );
	m_RenderableObject->SetScale( mathUtils::Lerp( current->GetScale(), next->GetScale(), t ) );
	m_RenderableObject->Update(0);
}


void CCinematicObject::Stop()
{
	CCinematicPlayer::Stop();
	m_CurrentKeyFrame = 0;
}


void CCinematicObject::OnRestartCycle()
{
}


