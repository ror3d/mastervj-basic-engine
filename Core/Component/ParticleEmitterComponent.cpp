#include "ParticleEmitterComponent.h"
#include "Scene/Element.h"

#include <Engine/Engine.h>
#include <Particles/ParticleSystemManager.h>
#include <Particles/ParticleSystemInstance.h>
#include <Graphics/Renderer/Renderer.h>

#include <Base/XML/XMLTreeNode.h>

#include "ComponentManager.h"

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

	m_CheckCollisionsEveryNParticles = node.GetIntProperty( "collisions_step", 0, false );
}


CParticleEmitterComponent::~CParticleEmitterComponent()
{}

void CParticleEmitterComponent::Init()
{
}

void CParticleEmitterComponent::FixedUpdate( float ElapsedTime )
{
	if ( m_CheckCollisionsEveryNParticles > 0 )
	{
		std::vector<std::string> hits = m_ParticleInstance->CheckCollisions( m_CheckCollisionsEveryNParticles );
		CComponentManager* cm = CEngine::GetSingleton().getComponentManager();
		auto owner = GetOwner();
		std::string myName = GetOwner()->getName();

		for ( auto &hit : hits )
		{
			auto cmp = cm->get( hit );
			if ( cmp )
			{
				cmp->GetOwner()->SendMsg( "OnParticleHit", owner );
				//owner->SendMsg( "OnParticleHit", cmp->GetOwner() );
			}
		}
	}
}

void CParticleEmitterComponent::Update(float ElapsedTime)
{
	m_ParticleInstance->SetEnabled(GetOwner()->GetEnabled());
	m_ParticleInstance->SetPosition( GetOwner()->GetPosition() );
	Vect3f rot = GetOwner()->GetRotation();
	/*Mat44f l_RotYaw;
	l_RotYaw.SetIdentity();
	l_RotYaw.RotByAngleY(-rot.x);
	Mat44f l_RotPitch;
	l_RotPitch.SetIdentity();
	l_RotPitch.RotByAngleX(-rot.y);
	Mat44f l_RotRoll;
	l_RotRoll.SetIdentity();
	l_RotRoll.RotByAngleZ(-rot.z);
	Mat44f mRot = l_RotRoll*l_RotPitch*l_RotYaw;*/
	m_ParticleInstance->SetStartSpeedRotation(Quatf::GetQuaternionFromRadians(rot));
	m_ParticleInstance->Update(ElapsedTime);
}

void CParticleEmitterComponent::Render(CContextManager&  _context)
{
	Mat44f mat;
	mat.SetIdentity();
	CEngine::GetSingleton().getRenderer()->AddRenderableToRender("particles", m_ParticleInstance, mat);
}


void CParticleEmitterComponent::Destroy()
{
}
