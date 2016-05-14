#include "CharControllerComponent.h"


CCharacterControllerComponent::CCharacterControllerComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CComponent(node, Owner)
{
	setName("_CharacterController");
}

CCharacterControllerComponent::CCharacterControllerComponent(CRenderableObject* Owner)
	: CComponent("_CharacterController", Owner)
{
}

CCharacterControllerComponent::~CCharacterControllerComponent()
{}

void CCharacterControllerComponent::Init()
{
}

void CCharacterControllerComponent::Destroy()
{
}


void CCharacterControllerComponent::Update(float ElapsedTime)
{
}

bool CCharacterControllerComponent::IsGrounded()
{
	return true;
}

void CCharacterControllerComponent::Move(const Vect3f& position, const Vect3f& up, float elapsedTime)
{
}

