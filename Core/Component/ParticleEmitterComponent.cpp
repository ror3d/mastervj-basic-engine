#include "ParticleEmitterComponent.h"
#include "Scene/Element.h"


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


void CParticleEmitterComponent::Destroy()
{
}
