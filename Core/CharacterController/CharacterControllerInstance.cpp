#include "CharacterControllerInstance.h"

#include <Core/Engine/Engine.h>
#include <Camera/FPSCameraController.h>
#include <Renderable/RenderableObject.h>
#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include "Animation/AnimatedInstanceModel.h"
#include <Scripting/ScriptManager.h>

CCharacterControllerInstance::CCharacterControllerInstance(std::string Name, CFPSCameraController * fpsCam, CRenderableObject * rendObjec) 
	: CNamed(Name)
	, m_fpsCamera(fpsCam)
	, m_objectModel(rendObjec)
{
	CEngine::GetSingleton().getPhysXManager()->createController(0.75f, 0.5f, 10, Vect3f(5, -0.8f, 5), Name);
}

CCharacterControllerInstance::~CCharacterControllerInstance()
{
}

void CCharacterControllerInstance::Update(float _elapsedTime)
{
	CEngine::GetSingleton().getScriptManager()->RunCode("CharTest()");	
}
