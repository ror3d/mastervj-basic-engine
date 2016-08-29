#include "AnimatedInstanceComponent.h"

#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Animation/AnimatedMeshManager.h>
#include <Graphics/Animation/AnimatedMeshInstance.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Renderer/Renderer.h>

#include <Base/XML/XMLTreeNode.h>


const std::string CAnimatedInstanceComponent::COMPONENT_TYPE = "AnimatedInstance";

CAnimatedInstanceComponent::CAnimatedInstanceComponent(const CAnimatedInstanceComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_animatedMeshCore = base.m_animatedMeshCore;
	CAnimatedMesh* animMesh = CEngine::GetSingleton().getAnimatedMeshManager()->get(base.m_animatedMeshCore);
	m_AnimatedMeshInstance = new CAnimatedMeshInstance(animMesh);
	m_layers = base.m_layers;
}

CAnimatedInstanceComponent::CAnimatedInstanceComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	std::string core = node.GetPszProperty("core", "", true);
	DEBUG_ASSERT(core != "");
	m_animatedMeshCore = core;
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

	float timeToStart = node.GetFloatProperty("startTime", -1, false);
	if (timeToStart != -1)
	{
		m_AnimatedMeshInstance->ClearCycle(0, 0.2);
		m_AnimatedMeshInstance->ExecuteAction(0, 0.2, 0, timeToStart, false);
	}
}


CAnimatedInstanceComponent::~CAnimatedInstanceComponent()
{
	delete m_AnimatedMeshInstance;
}

void CAnimatedInstanceComponent::Update(float ElapsedTime)
{
	m_AnimatedMeshInstance->Update(ElapsedTime);
}

void CAnimatedInstanceComponent::Render(CContextManager&  _context)
{
	if (!GetOwner()->GetEnabled())
	{
		return;
	}

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

void CAnimatedInstanceComponent::SetAnimationTime(float animationTime) const
{
	m_AnimatedMeshInstance->SetAnimationTime(animationTime);
}

