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

	std::string core = node.GetPszISOProperty("name_to_spawn", "", false);
	
	CEngine::GetSingleton().getSceneManager()->GetObjectById(core)->GetCharacterController()->SetPosition(Owner->GetPosition());
	CEngine::GetSingleton().getSceneManager()->GetObjectById(core)->SetYawPitchRoll(Owner->GetYaw(), Owner->GetPitch(), Owner->GetRoll());
	CEngine::GetSingleton().getSceneManager()->GetObjectById(core)->GetCamera()->Reset();
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
}

void CSpawnComponent::FixedUpdate(float ElapsedTime)
{
}

