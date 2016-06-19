#include "MeshInstanceComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Renderer/Renderer.h>

CMeshInstanceComponent::CMeshInstanceComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CMeshInstanceComponent::CMeshInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);

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
	// TODO Change to use the Renderer
	//CEffectManager::m_SceneParameters.m_World = GetOwner()->GetTransform();
	//m_StaticMesh->Render( &_context );
	for (auto &const layer : m_layers)
	{
		CEngine::GetSingleton().getRenderer()->AddRenderableToRender(layer, m_StaticMesh, GetOwner()->GetTransform());
	}
}

void CMeshInstanceComponent::Destroy()
{
}
