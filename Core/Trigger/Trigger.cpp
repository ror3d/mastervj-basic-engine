#include "Trigger.h"
#include "Core/Engine/Engine.h"

CTrigger::CTrigger(CXMLTreeNode Node) : CNamed(Node)
	, m_activated(false)
{
	m_once = Node.GetBoolProperty("ActivateTriggerOnce");
	m_action = Node.GetIntProperty("TriggerActionType");
	m_nameAction = Node.GetPszProperty("TriggerActionName");
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
	if (m_action == "Launch Cinematic")
	{
		CEngine::GetSingleton().getCinematicsActionManager()->LoadXML(m_nameAction);
	}
	else if (m_action == "Launch Animation")
	{

		//TODO
	}
	
}
