#include "ParticleEmitterComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Particles/ParticleSystemManager.h>
#include <Particles/ParticleSystemInstance.h>
#include <Graphics/Renderer/Renderer.h>

#include <Base/XML/XMLTreeNode.h>

const std::string CParticleEmitterComponent::COMPONENT_TYPE = "ParticleEmitter";

CParticleEmitterComponent::CParticleEmitterComponent(const CParticleEmitterComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_ParticleInstance = new CParticleSystemInstance( *base.m_ParticleInstance );
	m_ParticleInstance->SetEnabled(Owner->GetEnabled());
}

CParticleEmitterComponent::CParticleEmitterComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	
	SetNameFromParentName( Owner->getName() );

	m_ParticleInstance = new CParticleSystemInstance(node);
	m_ParticleInstance->SetEnabled(Owner->GetEnabled());
}


CParticleEmitterComponent::~CParticleEmitterComponent()
{}

void CParticleEmitterComponent::Init()
{
}

void CParticleEmitterComponent::Update(float ElapsedTime)
{
	m_ParticleInstance->SetEnabled(GetOwner()->GetEnabled());
	m_ParticleInstance->Update(ElapsedTime);
}

void CParticleEmitterComponent::Render(CContextManager&  _context)
{
	CEngine::GetSingleton().getRenderer()->AddRenderableToRender("particles", m_ParticleInstance, GetOwner()->GetTransform());
}


void CParticleEmitterComponent::Destroy()
{
}
