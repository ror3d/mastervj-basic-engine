#include "AnimatedInstanceComponent.h"

#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Animation/AnimatedMeshManager.h>
#include <Graphics/Animation/AnimatedMeshInstance.h>
#include <Graphics/Effect/EffectManager.h>

#include <Base/XML/XMLTreeNode.h>

CAnimatedInstanceComponent::CAnimatedInstanceComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CAnimatedInstanceComponent::CAnimatedInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
	std::string core = node.GetPszProperty("core", "", true);
	DEBUG_ASSERT(core != "");
	CAnimatedMesh* animMesh = CEngine::GetSingleton().getAnimatedMeshManager()->get(core);
	m_AnimatedMeshInstance = new CAnimatedMeshInstance(animMesh);
}


CAnimatedInstanceComponent::~CAnimatedInstanceComponent()
{}

void CAnimatedInstanceComponent::Update(float ElapsedTime)
{
	m_AnimatedMeshInstance->Update(ElapsedTime);
}

void CAnimatedInstanceComponent::Render(CContextManager&  _context)
{
	CEffectManager::m_SceneParameters.m_World = GetOwner()->GetTransform();
	m_AnimatedMeshInstance->Render( &_context );
}

void CAnimatedInstanceComponent::Destroy()
{
}
