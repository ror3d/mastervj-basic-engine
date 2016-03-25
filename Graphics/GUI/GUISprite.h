#pragma once

#include "GUIComponent.h"

class GUISprite : public CGUIComponent
{
public:

	GUISprite( CGUIContainer* parent, const CXMLTreeNode& node );

	~GUISprite();

	virtual void Update( float deltaTime );

	virtual void Render( CGUIRenderer* batch );
};
