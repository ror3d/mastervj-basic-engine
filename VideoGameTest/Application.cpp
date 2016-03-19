#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Renderable/RenderableObjectsManager.h>
#include "Animation/AnimatedInstanceModel.h"
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
	m_FixedCamera = false;
}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::~CApplication");
}

void CApplication::Update(float _ElapsedTime)
{
	CEngine& engine = CEngine::GetSingleton();
	m_Timer += _ElapsedTime;
	engine.getEffectsManager()->m_SceneParameters.m_Time = m_Timer;

	CEngine::GetSingleton().getPhysXManager()->update(_ElapsedTime);
	CEngine::GetSingleton().getLayerManager()->Update(_ElapsedTime);
	CEngine::GetSingleton().getCinematicsActionManager()->Update();
	CEngine::GetSingleton().getCinematicManager()->Update(_ElapsedTime);

	
	if (CInputManager::GetInputManager()->IsActionActive("FIXCAMERA"))
	{
		m_FixedCamera = m_FixedCamera ? false : true;
	}

	if (!m_FixedCamera)
	{
		if (CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName() == std::string("__fps"))
		{
			CEngine::GetSingleton().getCharacterControllerManager()->UpdateInstances(_ElapsedTime);
		}
		else if (CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName() == std::string("__debug"))
		{
			CEngine::GetSingleton().getCameraManager()->Update(_ElapsedTime);
		}
	}
}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
