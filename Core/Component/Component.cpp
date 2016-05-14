#include "Component.h"

#include <Base/XML/XMLTreeNode.h>

CComponent::CComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CNamed(node)
	, m_Owner(Owner)
{
	for (int i = 0; i < node.GetNumChildren(); ++i)
	{
		auto prop = node(i);
		if (prop.GetName() != std::string("property"))
		{
			return;
		}

		std::string name = prop.GetPszProperty("name", "", false);
		std::string value = prop.GetPszProperty("value", "", false);
		std::string type = prop.GetPszProperty("type", "string", false);
		m_properties.push_back({name, type, value});
	}
}

