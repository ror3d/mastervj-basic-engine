#include "RenderableObject.h"

#include "RenderableObjectTechnique.h"

#include "Mesh/RenderableVertexs.h"


#include <Core/Engine/Engine.h>
#include <Core/Component/ComponentManager.h>
#include <Core/Component/ScriptedComponent.h>

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
		if (comp.GetName() == std::string("scripted_component"))
		{
			std::string name = comp.GetPszProperty("class");
			auto component = new CScriptedComponent(name, this);
			cmgr->AddComponent(component);
			AddComponent(comp.GetName(), component);
		}

	}
}

void CRenderableObject::AddComponent(std::string Name, CComponent* component)
{
	m_componentContainer.add(Name, component);
}