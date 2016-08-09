#include "MeshInstanceComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Renderer/Renderer.h>

const std::string CMeshInstanceComponent::COMPONENT_TYPE = "MeshInstance";

CMeshInstanceComponent::CMeshInstanceComponent(const CMeshInstanceComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_StaticMesh = base.m_StaticMesh;
	m_layers = base.m_layers;
}

CMeshInstanceComponent::CMeshInstanceComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	SetEnabled(node.GetBoolProperty("visible", true, false));

	std::string coreName = node.GetPszProperty("core", "", false);
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(coreName);

	std::string layers = node.GetPszProperty("layers", "default", false);

	// separar layers i apuntar-se al render de cada un
	char prev = ' ';
	for (char c : layers)
	{
		if (c != ' ' && prev == ' ')
		{
			m_layers.push_back("");
		}

		if (c != ' ')
		{
			m_layers.back() += c;
		}
		prev = c;
	}
}

void CMeshInstanceComponent::SetStaticMesh(const std::string& coreName)
{
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(coreName);
}

CMeshInstanceComponent::~CMeshInstanceComponent()
{}

void CMeshInstanceComponent::Render(CContextManager&  _context)
{
	for (auto &const layer : m_layers)
	{
		CEngine::GetSingleton().getRenderer()->AddRenderableToRender(layer, m_StaticMesh, GetOwner()->GetTransform());
	}
}

void CMeshInstanceComponent::Destroy()
{
}
