#include "Trigger.h"
#include <XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Core/Logic/LogicManager.h>

CTrigger::CTrigger(CXMLTreeNode Node) : CNamed(Node)
	, m_activated(false)
{
	m_once = Node.GetBoolProperty("ActivateTriggerOnce");
	//m_action = Node.GetIntProperty("TriggerActionType");
	//m_nameAction = Node.GetPszProperty("TriggerActionName");
}

CTrigger::~CTrigger()
{

}

void CTrigger::Activate(){
	if (!m_activated)
	{
		m_activated = true;
		LaunchAction();
	}
	else if (!m_once)
	{
		LaunchAction();
	}
		
}

void CTrigger::LaunchAction()
{
	CEngine::GetSingleton().getLogicManager()->m_activedLogic->push_back(getName());
	/*if (m_action == "Launch Cinematic")
	{
		CEngine::GetSingleton().getCinematicsActionManager()->LoadXML(m_nameAction);
	}
	else if (m_action == "Launch Animation")
	{

		//TODO
	}*/
	
}

