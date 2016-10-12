#include "SpawnComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Core/Scene/SceneManager.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include "ComponentManager.h"

#include <Base/Scripting/LuaErrorCapture.h>
#include <selene.h>
#include <Base/Scripting/ScriptManager.h>

const std::string CSpawnComponent::COMPONENT_TYPE = "Spawn";

CSpawnComponent::CSpawnComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );
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
	auto sm = CEngine::GetSingleton().getScriptManager();
	auto& sel = *sm->getLuaState();

	CElement* owner = GetOwner();
	auto p = owner->GetPosition();
	auto q = owner->GetRotation();
	sel( ( "g_CharacterSpawnPosition = Vect3f.new(" + std::to_string( p.x ) + "," + std::to_string( p.y ) + "," + std::to_string( p.z ) + ")" ).c_str() );
	sel( ( "g_CharacterSpawnRotation = Vect3f.new(" + std::to_string( q.x ) + "," + std::to_string( q.y ) + "," + std::to_string( q.z ) + ")" ).c_str() );
	sel["g_CharacterSpawnSet"] = true;

	OutputDebugStringA( "Setting spawn point\n" );

}

