#include "GUIContainer.h"


CGUIContainer::CGUIContainer( CGUIContainer* parent, const CXMLTreeNode& node )
	: CGUIContainer(parent, node)
{
	// TODO: Init components
}


CGUIContainer::~CGUIContainer()
{
}



void CGUIContainer::UpdateRect( Rect parentPxRect )
{
	CGUIContainer::UpdateRect( parentPxRect );

	Rect r = GetPxRect();
	for ( auto &child : m_orderedComponents )
	{
		child->UpdateRect( r );
	}
}


void CGUIContainer::Update( float deltaTime )
{
	for ( auto &child : m_orderedComponents )
	{
		child->Update( deltaTime );
	}
}


void CGUIContainer::Render( CGUIRenderer* batch )
{
	for ( auto &child : m_orderedComponents )
	{
		child->Render( batch );
	}
}


