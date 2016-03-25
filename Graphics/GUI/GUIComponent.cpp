#include "GUIComponent.h"
#include "GUIContainer.h"

#include <Base/Utils/Utils.h>
#include <cmath>

CGUIComponent::CGUIComponent( CGUIContainer* parent, const CXMLTreeNode& node )
	: CNamed(node)
	, m_parent(parent)
{
	// TODO: Get values from node
}

CGUIComponent::~CGUIComponent()
{
}

void CGUIComponent::SetAnchors( Vect2f tl, Vect2f br )
{
	m_parentAnchors.topLeft = tl;
	m_parentAnchors.bottomRight = br;
}

void CGUIComponent::SetPxAnchors( Vect2f tl, Vect2f br )
{
	const Rect &r = m_parent->GetPxRect();
	tl.x = tl.x / r.w;
	tl.y = tl.y / r.h;
	br.x = br.x / r.w;
	br.y = br.y / r.h;
	m_parentAnchors.topLeft = Vect2f(std::fmin(tl.x, br.x), std::fmin(tl.y, br.y));
	m_parentAnchors.bottomRight = Vect2f(std::fmax(tl.x, br.x), std::fmax(tl.y, br.y));
}


void CGUIComponent::SetPivotPosition( Vect2f position )
{
	m_pivotPosition = position;
}

void CGUIComponent::SetPxPivotPosition( Vect2f position )
{
	const Rect &r = m_parent->GetPxRect();
	if ( m_parentAnchors.topLeft.x - m_parentAnchors.bottomRight.x > 0 )
	{
		m_pivotPosition.x = ( position.x / r.w - m_parentAnchors.topLeft.x ) / ( m_parentAnchors.topLeft.x - m_parentAnchors.bottomRight.x );
	}
	else
	{
		m_pivotPosition.x = position.x / r.w;
	}

	if ( m_parentAnchors.topLeft.y - m_parentAnchors.bottomRight.y > 0 )
	{
		m_pivotPosition.y = ( position.y / r.h - m_parentAnchors.topLeft.y ) / ( m_parentAnchors.topLeft.y - m_parentAnchors.bottomRight.y );
	}
	else
	{
		m_pivotPosition.y = position.y / r.h;
	}
}


void CGUIComponent::SetPivotOffset( Vect2f offset )
{
	m_pivotOffset = offset;
}

void CGUIComponent::SetPxPivotOffset( Vect2f offset )
{
	m_pivotOffset.x = offset.x / m_pxRect.w;
	m_pivotOffset.y = offset.y / m_pxRect.h;
}


void CGUIComponent::SetSize( Vect2f size )
{
	m_size = size;
}

void CGUIComponent::SetPxSize( Vect2f size )
{
	const Rect &r = m_parent->GetPxRect();
	if ( m_parentAnchors.topLeft.x - m_parentAnchors.bottomRight.x > 0 )
	{
		m_size.x = size.x / ( ( m_parentAnchors.topLeft.x - m_parentAnchors.bottomRight.x ) * r.w );
	}
	else
	{
		m_size.x = size.x / r.w;
	}

	if ( m_parentAnchors.topLeft.y - m_parentAnchors.bottomRight.y > 0 )
	{
		m_size.y = size.y / ( ( m_parentAnchors.topLeft.y - m_parentAnchors.bottomRight.y ) * r.h );
	}
	else
	{
		m_size.y = size.y / r.h;
	}
}

Rect CGUIComponent::GetPxRect() const
{
	return m_pxRect;
}


void CGUIComponent::UpdateRect( Rect parentPxRect )
{
	Vect2f anchorTL = parentPxRect.position + parentPxRect.size * m_parentAnchors.topLeft;
	Vect2f anchorBR = parentPxRect.position + parentPxRect.size * m_parentAnchors.bottomRight;

	Vect2f anchorSize = anchorBR - anchorTL;

	if ( anchorSize.x < 1.f/parentPxRect.w )
	{
		m_pxRect.w = m_size.x * parentPxRect.w;
	}
	else
	{
		m_pxRect.w = m_size.x * anchorSize.x;
	}
	if ( anchorSize.y < 1.f/parentPxRect.h )
	{
		m_pxRect.h = m_size.y * parentPxRect.w;
	}
	else
	{
		m_pxRect.h = m_size.y * anchorSize.x;
	}



	Vect2f pivotOffset = m_pivotOffset * m_pxRect.size;

	m_pxRect.position = anchorTL + anchorSize * m_pivotPosition + pivotOffset;
}

void CGUIComponent::SetParent( CGUIContainer * parent )
{
	m_parent = parent;
	// TODO: remove child from old parent and add to new one
}

