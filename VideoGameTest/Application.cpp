#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Renderer/RenderManager.h>
#include "Renderable/AnimatedInstanceModel.h"
#include "Animation/AnimatedCoreModel.h"

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <XML/XMLTreeNode.h>
#include <Graphics/Cinematics/Cinematic.h>

//#include <Base/Scripting/ScriptManager.h>

#include <Engine/Engine.h>

//CScriptManager *s_sm = nullptr;

CApplication::CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager )
	: m_RenderManager( _renderManager )
	, m_ContextManager( _ContextManager )
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");
	CDebugHelper::GetDebugHelper()->CreateMainBar();
	sendedInfoToAnim = false;
	activeMovAnim = false;
}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::~CApplication" );
}

void CApplication::CreateCharController()
{		
	CEngine::GetSingleton().getPhysicsManager()->createController(1.75f, 0.5f, 10, Vect3f(0, 0, 0), "main");
}

void CApplication::Update( float _ElapsedTime )
{
	CEngine::GetSingleton().getPhysicsManager()->update(_ElapsedTime);
	CEngine::GetSingleton().getLayerManager()->Update(_ElapsedTime);
	
	switch (m_RenderManager->getCurrentCameraNum())
	{
		case 0:
			if ( CInputManager::GetInputManager()->IsActionActive( "MOVE_CAMERA" ) )
			{
				Vect3f cameraMovement( 0, 0, 0 );

				cameraMovement.x = CInputManager::GetInputManager()->GetAxis( "X_AXIS" ) * 0.0005f;
				cameraMovement.y = CInputManager::GetInputManager()->GetAxis( "Y_AXIS" ) * 0.005f;				
				m_RenderManager->getSphericalCamera()->Update(cameraMovement);
			}
			break;
		case 1:
		{
				m_RenderManager->getFPSCamera()->SetPitch(m_RenderManager->getFPSCamera()->GetPitch() - m_RenderManager->getFPSCamera()->GetPitchDisplacement());
				if (CInputManager::GetInputManager()->GetAxis("STATICMOUSEAxis") != 1){
					m_RenderManager->getFPSCamera()->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * 0.0005f);
					m_RenderManager->getFPSCamera()->AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS")  * 0.005f);
					
				}
				if (m_RenderManager->getFPSCamera()->GetPitch() > 0.8f)//No atraviesa suelo
					m_RenderManager->getFPSCamera()->SetPitch(0.8f);
				if (m_RenderManager->getFPSCamera()->GetPitch() < -0.5f)//Vista superior personaje
					m_RenderManager->getFPSCamera()->SetPitch(-0.5f);
				
				float velMultiplier = 0.1f;
				Vect3f cameraMovement(0, 0, 0);
				float Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE")*velMultiplier;
				float Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD")*velMultiplier;
				float m_Yaw = m_RenderManager->getFPSCamera()->GetYaw();

				
				cameraMovement.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis")*velMultiplier;
				cameraMovement.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + 3.14159f*0.5f));
				cameraMovement.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + 3.14159f*0.5f));
				if ((Strafe ==0 && Forward ==0) && activeMovAnim){
					sendedInfoToAnim = false;
					((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main"))->ClearCycle(1, 0.2);
					((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main"))->BlendCycle(0, 1.0, 0.4);
					activeMovAnim = false;

				}
				else if ((Strafe != 0 || Forward != 0) && !activeMovAnim){
					((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main"))->ClearCycle(0, 0.05);
					((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main"))->BlendCycle(1, 1.0, 0.2);
					activeMovAnim = true;
				}
				
				cameraMovement = CEngine::GetSingleton().getPhysicsManager()->moveCharacterController(cameraMovement, m_RenderManager->getFPSCamera()->GetUp(), _ElapsedTime, "main");
				
				CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main")->SetPosition(cameraMovement);
				CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("main")->SetYaw(-m_Yaw+3.14159*0.5f);
				

				Vect3f rotacionDeseada = m_RenderManager->getFPSCamera()->GetCameraDisplacement();
				
				
				rotacionDeseada = rotacionDeseada.RotateZ(m_RenderManager->getFPSCamera()->GetPitch());
				rotacionDeseada = rotacionDeseada.RotateY(-m_Yaw);
				m_RenderManager->getFPSCamera()->SetPosition(cameraMovement + rotacionDeseada);
				m_RenderManager->getFPSCamera()->SetPitch(m_RenderManager->getFPSCamera()->GetPitch() + m_RenderManager->getFPSCamera()->GetPitchDisplacement());
				break;
		}
	}
}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
