#pragma once

#include <Base/Utils/Named.h>
#include <Base/Math/Vector2.h>

struct Rect
{
	union
	{
		struct
		{
			Vect2f position;
		};
		struct
		{
			float x, y;
		};
	};
	union
	{
		struct
		{
			Vect2f size;
		};
		struct
		{
			float w, h;
		};
	};

	Rect() {}
	Rect( Vect2f size ) : size( size ) {}
	Rect( Vect2f p, Vect2f s ) : position( p ), size( s ) {}
	Rect( float x, float y, float w, float h ) : position( x, y ), size( w, h ) {}

	Rect( const Rect& r ) : position(r.position), size(r.size) {}
};

class CGUIContainer;
class CGUIRenderer;

class CGUIComponent : public CNamed
{
	CGUIContainer * m_parent;

	/**
	 * Position of the anchors, relative to the top-left vertex of the containing rect.
	 */
	struct
	{
		Vect2f topLeft, bottomRight;
	} m_parentAnchors;

	/**
	 * Position of the pivot relative to the anchors or to the parent's rect if anchors are in the same line or point
	 */
	Vect2f m_pivotPosition;

	/**
	 * Position of the component relative to the pivot in range [0,1] normalized to m_pxRect
	 */
	Vect2f m_pivotOffset;

	/**
	 * Width and height of the component, in [0,1] normalized by m_parentAnchors or to the parent's rect if anchors are in the same line or point
	 */
	Vect2f m_size;

	/**
	 * Absolute rect of the component as calculated from the latest UpdateRect
	 */
	Rect m_pxRect;

public:

	CGUIComponent( CGUIContainer* parent, const CXMLTreeNode& node );

	virtual ~CGUIComponent();

	CGUIContainer * GetParent() const { return m_parent; }

	/**
	 * Set anchors, normalized, relative to the parent's rect
	 */
	void SetAnchors( Vect2f topLeft, Vect2f bottomRight );

	/**
	 * Set anchors, in pixels, relative to the parent's rect
	 */
	void SetPxAnchors( Vect2f topLeft, Vect2f bottomRight );


	/**
	 * Set pivot position relative to our own anchors or to the parent's rect if anchors are at the same line/point
	 */
	void SetPivotPosition( Vect2f position );

	/**
	 * Set pivot position, in pixels, relative to our own anchors or to the parent's rect if anchors are at the same line/point
	 */
	void SetPxPivotPosition( Vect2f position );


	/**
	 * Set the pivot offset relative to the top-left of the component's rect, normalized to the rect size
	 */
	void SetPivotOffset( Vect2f offset );

	/**
	 * Set the pivot offset relative to the top-left of the component's rect, in pixels
	 */
	void SetPxPivotOffset( Vect2f offset );

	/**
	 * Set the size of the component's rect, relative to the anchors
	 */
	void SetSize( Vect2f size );

	/**
	 * Set the size of the component's rect, in pixels
	 */
	void SetPxSize( Vect2f size );

	/**
	 * Get the current Rect in real pixels
	 */
	Rect GetPxRect() const;


	virtual void UpdateRect( Rect parentPxRect );


	virtual void Update( float deltaTime ) = 0;

	virtual void Render( CGUIRenderer* batch ) = 0;
};
