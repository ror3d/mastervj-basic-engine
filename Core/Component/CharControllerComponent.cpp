#include "CharControllerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

CCharacterControllerComponent::CCharacterControllerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);

	m_height = node.GetFloatProperty( "height", 1, false );
	m_radius = node.GetFloatProperty( "radius", 0.5, false );
	m_density = node.GetFloatProperty( "density", 1, false );
	m_offset = node.GetVect3fProperty( "offset", Vect3f(0, 0, 0), false );
}

CCharacterControllerComponent::CCharacterControllerComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
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

void CCharacterControllerComponent::SetPosition(const Vect3f& pos)
{
	CEngine::GetSingleton().getPhysXManager()->setPosition(pos, getName());
}

void CCharacterControllerComponent::Resize( float height, float radius )
{
	CEngine::GetSingleton().getPhysXManager()->resizeCharacterController(getName(), height, radius);
	float center = m_height / 2 + m_radius;
	float nCenter = height / 2 + radius;
	m_height = height;
	m_radius = radius;
}

float CCharacterControllerComponent::GetHeight()
{
	return m_height;
}

float CCharacterControllerComponent::GetRadius()
{
	return m_radius;
}