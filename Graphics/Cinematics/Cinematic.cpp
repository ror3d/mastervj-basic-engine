#include "Cinematic.h"
#include "CinematicObject.h"

#include <XML/XMLTreeNode.h>

CCinematic::CCinematic( CXMLTreeNode &treeNode )
	//: CRenderableObject(treeNode)
{
	m_Duration = treeNode.GetFloatProperty( "duration" );
	for (int i = 0; i < treeNode.GetNumChildren(); ++i)
	{
		auto co = new CCinematicObject(treeNode(i));
		m_CinematicObjects.push_back(co);
	}
}


CCinematic::~CCinematic()
{
	for ( auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it )
	{
		delete ( *it );
	}
}


void CCinematic::Stop()
{
	for (auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it)
	{
		(*it)->Stop();
	}
}


void CCinematic::Play()
{
	for (auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it)
	{
		(*it)->Play();
	}
}


void CCinematic::Pause()
{
	for (auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it)
	{
		(*it)->Pause();
	}
}


void CCinematic::AddCinematicObject( CCinematicObject *CinematicObject )
{
	m_CinematicObjects.push_back( CinematicObject );
}


void CCinematic::SetAnimTime( float Time )
{
	for ( auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it )
	{
		( *it )->SetAnimTime( Time );
	}
}

void CCinematic::Update( float ElapsedTime )
{
	for ( auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it )
	{
		( *it )->Update( ElapsedTime );
	}
}


void CCinematic::Render(CContextManager *_context)
{
	// Does nothing
}

