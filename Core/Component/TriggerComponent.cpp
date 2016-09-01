#include "TriggerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <PhysX/PhysXManager.h>
#include "ComponentManager.h"

const std::string CTriggerComponent::COMPONENT_TYPE = "Trigger";

CTriggerComponent::CTriggerComponent(CXMLTreeNode& node, CElement* Owner)
	: CPhysxComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_isTrigger = true;
	m_isKinematic = false;
	m_scale = node.GetVect3fProperty( "scale", Vect3f( 1, 1, 1 ), false );
	m_offset = node.GetVect3fProperty( "offset", Vect3f( 0, 0, 0 ), false );
}

CTriggerComponent::CTriggerComponent(const CTriggerComponent& base, CElement* Owner)
	: CPhysxComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_isTrigger = true;
	m_isKinematic = false;
	m_scale = base.m_scale;
	m_offset = base.m_offset;
}

CTriggerComponent::~CTriggerComponent()
{
}

void CTriggerComponent::Destroy()
{
	CPhysxComponent::Destroy();
}

void CTriggerComponent::Init()
{
	CPhysxComponent::Init(GetOwner()->GetScale().MulElems(m_scale), GetOwner()->GetPosition() + m_offset);
}


void CTriggerComponent::PhysxUpdate()
{
}

void CTriggerComponent::FixedUpdate(float ElapsedTime)
{
	if (!m_isStatic)
	{
		Move(GetOwner()->GetPosition() + m_offset);
	}

	auto elems = CEngine::GetSingleton().getPhysXManager()->getTriggerCollisions(getName());

	auto newElems = elems;

	std::vector<std::string> left;
	std::vector<std::string> active;

	for (auto &const e : m_activeElements)
	{
		newElems.erase(e);
		if (elems.find(e) == elems.end())
		{
			left.push_back(e);
		}
		else
		{
			active.push_back(e);
		}
	}

	m_activeElements.swap(active);


	auto cm = CEngine::GetSingleton().getComponentManager();

	auto own = GetOwner();
	for (auto &const e : left)
	{
		auto otherOwner = cm->get(e)->GetOwner();
		own->SendMsg("OnTriggerLeave", otherOwner);
	}

	for (auto &const e : m_activeElements)
	{
		auto otherOwner = cm->get(e)->GetOwner();
		own->SendMsg("OnTriggerStay", otherOwner);
	}

	for (auto &const e : newElems)
	{
		auto c = cm->get( e );
		if ( !c ) continue; // Wait until the component is registered

		auto otherOwner = c->GetOwner();
		own->SendMsg("OnTriggerEnter", otherOwner);
		m_activeElements.push_back( e );
	}
}

