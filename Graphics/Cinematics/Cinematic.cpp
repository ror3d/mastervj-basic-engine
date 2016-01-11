#include "Cinematic.h"
#include "CinematicObject.h"

#include <XML/XMLTreeNode.h>

CCinematic::CCinematic( CXMLTreeNode &treeNode )
	: CRenderableObject(treeNode)
{
	m_Duration = treeNode.GetFloatProperty( "duration" );
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
	// TODO
}


void CCinematic::Play( bool Cycle )
{
	// TODO
}


void CCinematic::Pause()
{
	// TODO
}


void CCinematic::AddCinematicObject( CCinematicObject *CinematicObject )
{
	m_CinematicObjects.push_back( CinematicObject );
}


void CCinematic::Update( float ElapsedTime )
{
	for ( auto it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it )
	{
		( *it )->Update( ElapsedTime );
	}
}


void CCinematic::Render( CRenderManager &RenderManager )
{
}


