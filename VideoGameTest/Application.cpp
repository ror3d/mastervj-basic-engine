#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Renderable/RenderableObjectsManager.h>
#include "Renderable/AnimatedInstanceModel.h"
#include "Animation/AnimatedCoreModel.h"

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <XML/XMLTreeNode.h>
#include <Graphics/Cinematics/Cinematic.h>

#include <PhysX/PhysXManager.h>
#include <Scripting/ScriptManager.h>

#include <Engine/Engine.h>

CScriptManager *s_sm = nullptr;

CApplication::CApplication(CContextManager *_ContextManager)
: m_ContextManager(_ContextManager)
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");
	CDebugHelper::GetDebugHelper()->CreateMainBar();
	activeMovAnim = false;
	}

		
CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::~CApplication" );
}

void CApplication::CreateCharController()
{
	CEngine::GetSingleton().getPhysXManager()->createController(1.75f, 0.5f, 10, Vect3f(0, 0, 0), "main");
}

void CApplication::Update( float _ElapsedTime )
{
	CEngine::GetSingleton().getPhysXManager()->update(_ElapsedTime);
	CEngine::GetSingleton().getLayerManager()->Update(_ElapsedTime);

	// TODO: move this to somewhere else! (like inside the FPS cam controller)
	ICameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CFPSCameraController* ccfps = dynamic_cast<CFPSCameraController*>( cc );
	if (ccfps != nullptr)
	{

		ccfps->SetPitch(ccfps->GetPitch() - ccfps->GetPitchDisplacement());
		if (CInputManager::GetInputManager()->GetAxis("STATICMOUSEAxis") != 1)
		{
			ccfps->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * 0.0005f);
			ccfps->AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS")  * 0.005f);
		}
		CRenderableObject * character = CEngine::GetSingleton().getLayerManager()->get("models")->get("main");
		if (character != nullptr)
		{
			if (ccfps->GetPitch() > 0.8f)//No atraviesa suelo
			{
				ccfps->SetPitch(0.8f);
			}
			if (ccfps->GetPitch() < -0.5f)//Vista superior personaje
			{
				ccfps->SetPitch(-0.5f);
			}
		}

		
		Vect3f cameraPosition(0, 0, 0);		
		float velMultiplier = 0.1f;
		float Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE");
		float Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD");
		float m_Yaw = ccfps->GetYaw();
				
		cameraPosition.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + 3.14159f*0.5f));
		cameraPosition.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + 3.14159f*0.5f));
		cameraPosition.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis");
		
		cameraPosition = CEngine::GetSingleton().getPhysXManager()->moveCharacterController(cameraPosition*velMultiplier, ccfps->GetUp(), _ElapsedTime, "main");

		if (character != nullptr)
		{
			character->SetPosition(cameraPosition);
			character->SetYaw(-m_Yaw + 3.14159*0.5f);

			Vect3f rotacionDeseada = ccfps->GetCameraDisplacement();
			rotacionDeseada = rotacionDeseada.RotateZ(ccfps->GetPitch());
			rotacionDeseada = rotacionDeseada.RotateY(-m_Yaw);

			ccfps->SetPosition(cameraPosition + rotacionDeseada);
			ccfps->SetPitch(ccfps->GetPitch() + ccfps->GetPitchDisplacement());

			if ((Strafe == 0 && Forward == 0) && activeMovAnim)
			{
				((CAnimatedInstanceModel*)character)->ClearCycle(1, 0.2);
				((CAnimatedInstanceModel*)character)->BlendCycle(0, 1.0, 0.4);
				activeMovAnim = false;
			}
			else if ((Strafe != 0 || Forward != 0) && !activeMovAnim)
			{
				((CAnimatedInstanceModel*)character)->ClearCycle(0, 0.05);
				((CAnimatedInstanceModel*)character)->BlendCycle(1, 1.0, 0.2);
				activeMovAnim = true;
			}
		}
		else{
			ccfps->SetPosition(cameraPosition);
			ccfps->SetPitch(ccfps->GetPitch() + ccfps->GetPitchDisplacement());
		}
	}	

	CEngine::GetSingleton().getCameraManager()->Update( _ElapsedTime );
}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
