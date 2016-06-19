#include "ParticleEmitterComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Graphics/Renderer/Renderer.h>

CParticleEmitterComponent::CParticleEmitterComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CParticleEmitterComponent::CParticleEmitterComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
}


CParticleEmitterComponent::~CParticleEmitterComponent()
{}

void CParticleEmitterComponent::Init()
{
}

void CParticleEmitterComponent::Update(float ElapsedTime)
{
}

void CParticleEmitterComponent::Render(CContextManager&  _context)
{
	//CEngine::GetSingleton().getRenderer()->AddRenderableToRender("particles", m_ParticleInstance, GetOwner()->GetTransform());
}


void CParticleEmitterComponent::Destroy()
{
}
