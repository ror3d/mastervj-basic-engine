#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Camera/FPSCameraController.h>

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics/Cinematics/Cinematic.h>

#include <PhysX/PhysXManager.h>

#include <Base/Scripting/ScriptManager.h>

#include <Engine/Engine.h>


static float s_mouseSpeed = 1;

CScriptManager *s_sm = nullptr;
static CPhysXManager* phMgr = nullptr;

static void __stdcall SwitchCameraCallback( void* _app )
{
	std::string cam = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName();
	if ( cam == "__debug" ) cam = "__fps";
	else if ( cam == "__fps" ) cam = "__debug";

	CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController(cam);
}

static void __stdcall ReloadScene(void* _app)
{
	CEngine::GetSingleton().getEffectsManager()->Reload();
	CEngine::GetSingleton().getMaterialManager()->reload();
	CEngine::GetSingleton().getStaticMeshManager()->Reload();
	CEngine::GetSingleton().getAnimatedModelManager()->Reload();
	CEngine::GetSingleton().getLayerManager()->Reload();
	CEngine::GetSingleton().getLightManager()->reload();
	CEngine::GetSingleton().getSceneRendererCommandManager()->Reload();
}

static void __stdcall CreateScene(void* a)
{
	CPhysXManager::ShapeDesc desc;
	desc.shape = CPhysXManager::ShapeDesc::Shape::Box;
	desc.density = 1;
	desc.material = "box";
	desc.size = Vect3f(1, 1, 1);
	desc.position = Vect3f(0, 0.5f, 0);
	phMgr->createActor("boxCol", CPhysXManager::ActorType::Static, desc);
}

static void __stdcall CreateChar(void* a)
{

}

CApplication::CApplication( CContextManager *_ContextManager)
	: m_ContextManager( _ContextManager )
	, m_Timer(0)
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::CApplication" );

	CDebugHelper::SDebugBar bar;
	bar.name = "CApplication";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "background";
		var.type = CDebugHelper::COLOR;
		var.mode = CDebugHelper::READ_WRITE;
		var.pColor = &_ContextManager->m_BackgroundColor;

		bar.variables.push_back(var);
	}
	/*
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "cube";
		var.type = CDebugHelper::POSITION_ORIENTATION;
		var.mode = CDebugHelper::READ_WRITE;
		var.pPositionOrientation = m_Cube.GetPtrTransform();

		bar.variables.push_back(var);
	}
	*/
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "switch camera";
		var.type = CDebugHelper::BUTTON;
		var.callback = SwitchCameraCallback;
		var.data = this;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "mouse speed";
		var.type = CDebugHelper::FLOAT;
		var.mode = CDebugHelper::READ_WRITE;
		var.pFloat = &s_mouseSpeed;
		var.params = " min=0.1 max=10 step=0.1 precision=1 ";

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "PhysX: Create scene";
		var.type = CDebugHelper::BUTTON;
		var.callback = CreateScene;
		var.data = this;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "PhysX: Create char";
		var.type = CDebugHelper::BUTTON;
		var.callback = CreateChar;
		var.data = this;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Reload Options";
		var.type = CDebugHelper::STRING;
		var.pString = "";

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "reload scene";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadScene;
		var.data = this;

		bar.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::~CApplication" );
}

void CApplication::Init()
{
	phMgr = CEngine::GetSingleton().getPhysXManager();
	phMgr->registerMaterial("ground", 1, 0.9, 0.1);
	phMgr->registerMaterial("box", 1, 0.9, 0.8);
	phMgr->registerMaterial("controller_material", 10, 2, 0.5);
	phMgr->createPlane("ground", "ground", Vect4f(0, 1, 0, 0));
	phMgr->createController(2, 0.5f, 10, Vect3f(0, 3, 0), "main");
}


void CApplication::Update( float _ElapsedTime )
{
	m_Timer += _ElapsedTime;
	CEngine::GetSingleton().getEffectsManager()->m_SceneParameters.m_Time = m_Timer;
	phMgr->update(_ElapsedTime);
	CEngine::GetSingleton().getLayerManager()->Update(_ElapsedTime);

	( (CInputManagerImplementation*)CInputManager::GetInputManager() )->SetMouseSpeed( s_mouseSpeed );

	// TODO: move this to somewhere else! (like inside the FPS cam controller)
	CCameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CFPSCameraController* ccfps = dynamic_cast<CFPSCameraController*>( cc );
	if(ccfps != nullptr)
	{
		Vect3f cameraMovement(0, 0, 0);
		float Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE");
		float Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD");
		float m_Yaw = ccfps->GetYaw();

		cameraMovement.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis");
		cameraMovement.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + 3.14159f*0.5f));
		cameraMovement.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + 3.14159f*0.5f));

		cameraMovement = phMgr->moveCharacterController(cameraMovement*0.2, ccfps->GetUp(), _ElapsedTime);
		ccfps->SetPosition(cameraMovement);
	}

	CEngine::GetSingleton().getCameraManager()->Update( _ElapsedTime );

}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
