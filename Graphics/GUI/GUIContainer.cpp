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

	// TODO update children
}


void CGUIContainer::Update( float deltaTime )
{
	// TODO
}


void CGUIContainer::Render()
{
	// TODO
}


