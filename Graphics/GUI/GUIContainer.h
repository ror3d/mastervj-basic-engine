#pragma once

#include "GUIComponent.h"

#include <string>
#include <vector>
#include <map>


class CGUIContainer : public CGUIComponent
{
	typedef std::map<std::string, uint32> ComponentsMap_t;
	typedef std::vector<CGUIComponent*> ComponentsVec_t;

	ComponentsMap_t m_components;
	ComponentsVec_t m_orderedComponents;

public:

	CGUIContainer( CGUIContainer* parent, const CXMLTreeNode& node );

	~CGUIContainer();


	virtual void UpdateRect( Rect parentPxRect );

	virtual void Update( float deltaTime );

	virtual void Render();
};
