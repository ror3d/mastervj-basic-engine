#include "Component.h"

#include <Base/XML/XMLTreeNode.h>

#include <Core/Engine/Engine.h>
#include <Core/Component/ComponentManager.h>

CComponent::CComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CNamed(node)
	, m_Owner(Owner)
	, m_Initiaized(false)
{
	DEBUG_ASSERT( Owner != nullptr );
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

CComponent::CComponent( const std::string& Name, CRenderableObject* Owner )
	: CNamed(Name)
	, m_Owner(Owner)
	, m_Initiaized(false)
{
}

void CComponent::Initialize()
{
	if ( CEngine::GetSingleton().getComponentManager()->IsInitialized() )
	{
		Init();
		m_Initiaized = true;
	}
}

void CComponent::ObjectInitialized()
{
	if ( m_Initiaized )
	{
		OnObjectInitialized();
	}
}
