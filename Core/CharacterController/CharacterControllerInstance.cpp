#include "CharacterControllerInstance.h"

#include <Core/Engine/Engine.h>
#include <Camera/FPSCameraController.h>
#include <Renderable/RenderableObject.h>
#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include "Animation/AnimatedInstanceModel.h"

CCharacterControllerInstance::CCharacterControllerInstance(std::string Name, CFPSCameraController * fpsCam, CRenderableObject * rendObjec) 
	: CNamed(Name)
	, m_fpsCamera(fpsCam)
	, m_objectModel(rendObjec)
	, m_activeMovAnim(false)
{
	CEngine::GetSingleton().getPhysXManager()->createController(1.75f, 0.5f, 10, Vect3f(0, 1, 0), Name);
}

CCharacterControllerInstance::~CCharacterControllerInstance()
{
}

void CCharacterControllerInstance::Update(float _elapsedTime)
{
	bool movement = Move(_elapsedTime);
	AnimateModel(movement);
}

bool CCharacterControllerInstance::Move(float _elapsedTime)
{
	float Strafe = 0;
	float Forward = 0;
	if (CInputManager::GetInputManager()->GetAxis("STATICMOUSEAxis") != 1) //TECLA F CONGELA MOV
	{
		Vect3f cameraPosition(0, 0, 0);
		float velMultiplier = 0.1f;
		Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE");
		Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD");
		float m_Yaw = m_fpsCamera->GetYaw();

		cameraPosition.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + 3.14159f*0.5f));
		cameraPosition.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + 3.14159f*0.5f));
		cameraPosition.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis");

		cameraPosition = CEngine::GetSingleton().getPhysXManager()->moveCharacterController(
			cameraPosition*velMultiplier, m_fpsCamera->GetUp(), _elapsedTime, "main");
		m_fpsCamera->SetPosition(cameraPosition);
	
		CEngine::GetSingleton().getCameraManager()->Update(_elapsedTime);
		m_objectModel->SetPosition(cameraPosition);
		m_objectModel->SetYaw(-m_Yaw + 3.14159*0.5f); //DEFORMED
	}
	return (Strafe == 0 && Forward == 0);
}

void CCharacterControllerInstance::AnimateModel(bool inMovement)
{
	if (inMovement && m_activeMovAnim)
	{
		((CAnimatedInstanceModel*)m_objectModel)->ClearCycle(1, 0.2);
		((CAnimatedInstanceModel*)m_objectModel)->BlendCycle(0, 1.0, 0.4);
		m_activeMovAnim = false;
	}
	else if (!inMovement && !m_activeMovAnim)
	{
		((CAnimatedInstanceModel*)m_objectModel)->ClearCycle(0, 0.05);
		((CAnimatedInstanceModel*)m_objectModel)->BlendCycle(1, 1.0, 0.2);
		m_activeMovAnim = true;
	}
}
