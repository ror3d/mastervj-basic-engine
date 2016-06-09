#include "MeshInstanceComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/StaticMesh.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Effect/EffectManager.h>

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

	// Todo: separar layers i apuntar-se al render de cada un
}

void CMeshInstanceComponent::SetStaticMesh(const std::string& coreName)
{
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(coreName);
}

CMeshInstanceComponent::~CMeshInstanceComponent()
{}

void CMeshInstanceComponent::Render(CContextManager&  _context)
{
	CEffectManager::m_SceneParameters.m_World = GetOwner()->GetTransform();
	m_StaticMesh->Render( &_context );
}

void CMeshInstanceComponent::Destroy()
{
}
