#include "ParticleEmitterComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Particles/ParticleSystemManager.h>
#include <Particles/ParticleSystemInstance.h>
#include <Graphics/Renderer/Renderer.h>

#include <Base/XML/XMLTreeNode.h>

CParticleEmitterComponent::CParticleEmitterComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CParticleEmitterComponent::CParticleEmitterComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);

	m_ParticleInstance = new CParticleSystemInstance(node);
}


CParticleEmitterComponent::~CParticleEmitterComponent()
{}

void CParticleEmitterComponent::Init()
{
}

void CParticleEmitterComponent::Update(float ElapsedTime)
{
	m_ParticleInstance->Update(ElapsedTime);
}

void CParticleEmitterComponent::Render(CContextManager&  _context)
{
	CEngine::GetSingleton().getRenderer()->AddRenderableToRender("particles", m_ParticleInstance, GetOwner()->GetTransform());
}


void CParticleEmitterComponent::Destroy()
{
}
