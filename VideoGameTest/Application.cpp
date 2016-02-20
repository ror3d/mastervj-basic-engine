#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Renderer/RenderManager.h>
#include "Renderable/AnimatedInstanceModel.h"

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <XML/XMLTreeNode.h>
#include <Graphics/Cinematics/Cinematic.h>

#include <Base/Scripting/ScriptManager.h>

#include <Engine/Engine.h>

CScriptManager *s_sm = nullptr;

CApplication::CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager )
	: m_RenderManager( _renderManager )
	, m_ContextManager( _ContextManager )
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");
	CDebugHelper::GetDebugHelper()->CreateMainBar();
}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::~CApplication" );
}

void CApplication::CreateCharController()
{	
	CEngine::GetSingleton().getPhysicsManager()->registerMaterial("ground", 1, 0.9, 0.1);
	CEngine::GetSingleton().getPhysicsManager()->registerMaterial("box", 1, 0.9, 0.8);
	CEngine::GetSingleton().getPhysicsManager()->registerMaterial("controller_material", 10, 2, 0.5);
	CEngine::GetSingleton().getPhysicsManager()->createPlane("ground", "ground", Vect4f(0, 1, 0, 0));
	CEngine::GetSingleton().getPhysicsManager()->createController(2, 0.5f, 10, Vect3f(0, 3, 0), "main");
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
			if (CInputManager::GetInputManager()->GetAxis("STATICMOUSEAxis") != 1){
				m_RenderManager->getFPSCamera()->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * 0.0005f);
				m_RenderManager->getFPSCamera()->AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * 0.005f);
			}		
			float velMultiplier = 0.3f;
			Vect3f cameraMovement(0, 0, 0);						
			float Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE")*velMultiplier;
			float Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD")*velMultiplier;
			float m_Yaw = m_RenderManager->getFPSCamera()->GetYaw();
			
			cameraMovement.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis")*velMultiplier;
			cameraMovement.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + 3.14159f*0.5f));
			cameraMovement.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + 3.14159f*0.5f));
			if (cameraMovement == Vect3f(0, 0, 0)){
				((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("bruja"))->BlendCycle(1, 1.0, 0.0);
			}
			else{
				((CAnimatedInstanceModel*)CEngine::GetSingleton().getLayerManager()->getDefaultLayer()->get("bruja"))->BlendCycle(0, 1.0, 0.0);
			}
			cameraMovement = CEngine::GetSingleton().getPhysicsManager()->moveCharacterController(cameraMovement, m_RenderManager->getFPSCamera()->GetUp(), _ElapsedTime);
			m_RenderManager->getFPSCamera()->SetPosition(cameraMovement);
		}
		break;
	}
}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
