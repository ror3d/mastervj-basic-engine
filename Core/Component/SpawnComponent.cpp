#include "SpawnComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Core/Scene/SceneManager.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include "ComponentManager.h"

const std::string CSpawnComponent::COMPONENT_TYPE = "Spawn";

CSpawnComponent::CSpawnComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_objName = node.GetPszISOProperty( "name_to_spawn", "", false );
}

CSpawnComponent::CSpawnComponent(const CSpawnComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );
}

CSpawnComponent::~CSpawnComponent()
{
}

void CSpawnComponent::Destroy()
{
}

void CSpawnComponent::Init()
{
	CElement* obj = CEngine::GetSingleton().getSceneManager()->GetObjectById( m_objName );
	if ( obj )
	{
		CElement* owner = GetOwner();
		obj->GetCharacterController()->SetPosition( owner->GetPosition() );
		obj->SetYawPitchRoll( owner->GetYaw(), owner->GetPitch(), owner->GetRoll() );
		obj->GetCamera()->Reset();
	}
}

void CSpawnComponent::FixedUpdate(float ElapsedTime)
{
}

