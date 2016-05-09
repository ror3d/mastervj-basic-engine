#include "CharacterControllerInstance.h"

#include <Core/Engine/Engine.h>
#include <Camera/FPSCameraController.h>
#include <Renderable/RenderableObject.h>
#include <Scripting/ScriptManager.h>
#include <Core/Component/ScriptedComponent.h>

CCharacterControllerInstance::CCharacterControllerInstance(std::string Name, CFPSCameraController * fpsCam, CRenderableObject * rendObjec) 
	: CNamed(Name)
	, m_fpsCamera(fpsCam)
	, m_objectModel(rendObjec)
{
	//CEngine::GetSingleton().getScriptManager()->RunCode("InitComponentManager()");
}

CCharacterControllerInstance::~CCharacterControllerInstance()
{
}

void CCharacterControllerInstance::Update(float _elapsedTime)
{
//	CScriptedComponent * comp = (CScriptedComponent*) m_objectModel->GetComponentManager()->get("ScriptedCharacterController");
//	comp->Update(_elapsedTime);
}
