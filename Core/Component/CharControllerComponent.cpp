#include "CharControllerComponent.h"

#include <Graphics/Renderable/RenderableObject.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

CCharacterControllerComponent::CCharacterControllerComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CComponent(node, Owner)
{
	setName(Owner->getName() + "_CharacterController");

	m_height = node.GetFloatProperty( "height", 1, false );
	m_radius = node.GetFloatProperty( "radius", 0.5, false );
	m_density = node.GetFloatProperty( "density", 1, false );
	m_offset = node.GetVect3fProperty( "offset", Vect3f(0, 0, 0), false );
}

CCharacterControllerComponent::CCharacterControllerComponent(CRenderableObject* Owner)
	: CComponent(Owner->getName() + "_CharacterController", Owner)
{
}

CCharacterControllerComponent::~CCharacterControllerComponent()
{}

void CCharacterControllerComponent::Init()
{
	CEngine::GetSingleton().getPhysXManager()->createController(m_height, m_radius, m_density, GetOwner()->GetPosition() - m_offset, getName());
}

void CCharacterControllerComponent::Destroy()
{
	CEngine::GetSingleton().getPhysXManager()->releaseCharacterController( getName() );
}


void CCharacterControllerComponent::FixedUpdate(float ElapsedTime)
{
}

bool CCharacterControllerComponent::IsGrounded()
{
	return CEngine::GetSingleton().getPhysXManager()->isCharacterControllerGrounded( getName() );
}

void CCharacterControllerComponent::Move(const Vect3f& velocity, const Vect3f& up, float elapsedTime)
{
	GetOwner()->SetPosition(CEngine::GetSingleton().getPhysXManager()->moveCharacterController( velocity, up, elapsedTime, getName() ) + m_offset);
}

