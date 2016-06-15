#include "TriggerManager.h"
#include "XML/XMLTreeNode.h"
#include "Trigger.h"
#include <Core/Engine/Engine.h>
#include <Core\Component\TriggerComponent.h>
#include <Math/Math.h>

CTriggerManager::CTriggerManager()
{
}

void CTriggerManager::AddElement(CTriggerComponent * comp)
{
	add(comp);
}
void CTriggerManager::Activate(std::string name)
{
	std::string nameToDelete = "_PhysxComponent";
	
	std::string::size_type i = name.find(nameToDelete);

	if (i != std::string::npos)
		name.erase(i, nameToDelete.length());

	std::string nameTrigger = name + "_TriggerComponent";
	get(nameTrigger)->SetStateTrigger(1);
	activedTriggers.push_back(nameTrigger);
}

void CTriggerManager::Deactivate(std::string name)
{
	std::string nameToDelete = "_PhysxComponent";

	std::string::size_type i = name.find(nameToDelete);

	if (i != std::string::npos)
		name.erase(i, nameToDelete.length());

	std::string nameTrigger = name + "_TriggerComponent";
	get(nameTrigger)->SetStateTrigger(3);
	activedTriggers.erase(std::find(activedTriggers.begin(), activedTriggers.end(), nameTrigger));
}


CTriggerManager::~CTriggerManager()
{
}
