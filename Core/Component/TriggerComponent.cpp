#include "TriggerComponent.h"

#include <Graphics/Renderable/RenderableObject.h>
#include <Core\Trigger\TriggerManager.h>
#include <Core/Engine/Engine.h>

CTriggerComponent::CTriggerComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CComponent(node, Owner)
{
	setName(Owner->getName() + "_TriggerComponent");
	m_EnterTrigger = false;
	m_StayTrigger = false;
	m_ExitTrigger = false;
	m_name = node.GetPszProperty("name", "nullname");

	CEngine::GetSingleton().getTriggerManager()->AddElement(this);
}

CTriggerComponent::CTriggerComponent(CRenderableObject* Owner)
	: CComponent(Owner->getName() + "_TriggerComponent", Owner)
{
}

CTriggerComponent::~CTriggerComponent()
{}

void CTriggerComponent::Init()
{
	
}

void CTriggerComponent::Destroy()
{
}


void CTriggerComponent::FixedUpdate(float ElapsedTime)
{
}

void CTriggerComponent::Activate()
{
	/*for (auto it = m_properties.begin(); it < m_properties.end(); it++)
	{
		if (it->name == getName())
		{
			it->value = true;
			break;
		}
	}*/
	
}

int CTriggerComponent::GetStateTrigger()
{
	if (m_EnterTrigger)
		return 1;
	else if (m_StayTrigger)
		return 2;
	else if (m_ExitTrigger)
		return 3;
	else
		return 0;
}

void CTriggerComponent::SetStateTrigger(int num){
	m_EnterTrigger = false;
	m_StayTrigger = false;
	m_ExitTrigger = false;

	if (num == 1)
	{
		m_EnterTrigger = true;
	}
	else if (num == 2)
	{
		m_StayTrigger = true;
	}
	else if (num == 3)
	{
		m_ExitTrigger = true;
	}
}
