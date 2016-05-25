#include "RenderableObject.h"

#include "RenderableObjectTechnique.h"

#include "Mesh/RenderableVertexs.h"


#include <Core/Engine/Engine.h>
#include <Core/Component/ComponentManager.h>
#include <Core/Component/Component.h>
#include <Core/Component/ScriptedComponent.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include <Core/Component/PhysxComponent.h>
#include <Core\Component\TriggerComponent.h>

CRenderableObject::CRenderableObject()
	: CNamed("")
{
}

CRenderableObject::CRenderableObject(CXMLTreeNode& treeNode)
	: C3DElement(treeNode)
	, CNamed(treeNode)
{
	auto cmgr = CEngine::GetSingleton().getComponentManager();
	for (int i = 0; i < treeNode.GetNumChildren(); ++i)
	{
		auto comp = treeNode(i);
		std::string type = comp.GetName();
		CComponent* component;
		if (type == "scripted_component")
		{
			std::string name = comp.GetPszProperty("class");
			component = new CScriptedComponent(comp, this);
		}
		else if (type == "character_controller")
		{
			component = new CCharacterControllerComponent(comp, this);
		}
		else if (type == "fps_camera")
		{
			component = new CFPSCameraComponent(comp, this);
		}
		else if (type == "physx")
		{
			component = new CPhysxComponent(comp, this, treeNode.GetPszProperty("core_name"));
		}
		else if (type == "trigger")
		{
			component = new CTriggerComponent(comp, this);
		}
		else
		{
			OutputDebugString("Component type not recognized\n");
			continue;
		}
		cmgr->AddComponent(component);
		AddComponent(component->getName(), component);
	}
	for ( auto const& comp : m_componentContainer )
	{
		comp.second->ObjectInitialized();
	}
}

void CRenderableObject::AddComponent(std::string Name, CComponent* component)
{
	m_componentContainer.add(Name, component);
	component->Initialize();
}

void CRenderableObject::SendMsg(const std::string msg)
{
	for (auto it = m_componentContainer.begin(); it != m_componentContainer.end(); it++)
	{
		it->second->SendMsg(msg);
	}
}

CFPSCameraComponent* CRenderableObject::GetCamera()
{
	auto comp = m_componentContainer.get(getName() + "_Camera");
	if (comp)
	{
		return dynamic_cast<CFPSCameraComponent*>(comp);
	}
	return nullptr;
}

CCharacterControllerComponent* CRenderableObject::GetCharacterController()
{
	auto comp = m_componentContainer.get(getName() + "_CharacterController");
	if (comp)
	{
		return dynamic_cast<CCharacterControllerComponent*>(comp);
	}
	return nullptr;
}

CPhysxComponent * CRenderableObject::GetPhysxComponent()
{
	auto comp = m_componentContainer.get(getName() + "_PhysxComponent");
	if (comp)
	{
		return dynamic_cast<CPhysxComponent*>(comp);
	}
	return nullptr;
}

CTriggerComponent * CRenderableObject::GetTriggerComponent()
{
	auto comp = m_componentContainer.get(getName() + "_TriggerComponent");
	if (comp)
	{
		return dynamic_cast<CTriggerComponent*>(comp);
	}
	return nullptr;
}
