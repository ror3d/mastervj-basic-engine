#include "Component.h"

#include <Base/XML/XMLTreeNode.h>

#include <Core/Engine/Engine.h>
#include <Core/Component/ComponentManager.h>

#include "Scene/Element.h"

CComponent::CComponent(CXMLTreeNode& node, CElement* Owner)
	: CNamed(node)
	, m_Owner(Owner)
	, m_Initialized(false)
	, m_Destroyed(false)
{
	DEBUG_ASSERT( Owner != nullptr );

	m_Enabled = node.GetBoolProperty("enabled", true, false);

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
		if ( type == "array" )
		{
			value = "";
			for ( int i = 0; i < prop.GetNumChildren(); ++i )
			{
				auto entry = prop( i );
				if ( entry.GetName() != std::string( "entry" ) )
				{
					continue;
				}

				std::string subVal = entry.GetPszProperty( "value", "", false );
				std::string subType = entry.GetPszProperty( "type", "string", false );

				if ( subType == "string" )
				{
					subVal = "\"" + subVal + "\"";
				}
				if ( value != "" )
				{
					value += ",";
				}
				value += subVal;
			}
			value = "[" + value + "]";
		}
		m_properties.push_back({name, type, value});
	}
}

CComponent::CComponent( const CComponent& base, CElement* Owner )
	: m_Owner(Owner)
	, m_Initialized(false)
	, m_Destroyed(false)
{
	DEBUG_ASSERT( Owner != nullptr );

	m_Enabled = base.m_Enabled;

	m_properties = base.m_properties;
}

void CComponent::Initialize()
{
	if ( CEngine::GetSingleton().getComponentManager()->IsInitialized() )
	{
		Init();
		m_Initialized = true;
	}
}

void CComponent::ObjectInitialized()
{
	if ( m_Initialized )
	{
		OnObjectInitialized();
	}
}

void CComponent::SetNameFromParentName( const std::string & name )
{
	CNamed::setName( name + "_" + this->GetComponentType() );
}
