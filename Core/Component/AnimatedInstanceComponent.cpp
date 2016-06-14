#include "AnimatedInstanceComponent.h"

#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Animation/AnimatedMeshManager.h>
#include <Graphics/Animation/AnimatedMeshInstance.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Renderer/Renderer.h>

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


CAnimatedInstanceComponent::~CAnimatedInstanceComponent()
{
}

void CAnimatedInstanceComponent::Update(float ElapsedTime)
{
	m_AnimatedMeshInstance->Update(ElapsedTime);
}

void CAnimatedInstanceComponent::Render(CContextManager&  _context)
{
	//CEffectManager::m_SceneParameters.m_World = GetOwner()->GetTransform();
	//m_AnimatedMeshInstance->Render( &_context );
	for (auto &const layer : m_layers)
	{
		CEngine::GetSingleton().getRenderer()->AddRenderableToRender(layer, m_AnimatedMeshInstance, GetOwner()->GetTransform());
	}
}

void CAnimatedInstanceComponent::Destroy()
{
}

void CAnimatedInstanceComponent::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{
	m_AnimatedMeshInstance->ExecuteAction(Id, DelayIn, DelayOut, WeightTarget, AutoLock);
}

void CAnimatedInstanceComponent::BlendCycle(int Id, float Weight, float DelayIn)
{
	m_AnimatedMeshInstance->BlendCycle(Id, Weight, DelayIn);
}

void CAnimatedInstanceComponent::ClearCycle(int Id, float DelayOut)
{
	m_AnimatedMeshInstance->ClearCycle(Id, DelayOut);
}

bool CAnimatedInstanceComponent::IsCycleAnimationActive(int Id) const
{
	return m_AnimatedMeshInstance->IsCycleAnimationActive(Id);
}

bool CAnimatedInstanceComponent::IsActionAnimationActive(int Id) const
{
	return m_AnimatedMeshInstance->IsActionAnimationActive(Id);
}

