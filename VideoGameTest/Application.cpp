#include "Application.h"

#include <Base/Math/Math.h>

#include <Core/Engine/Engine.h>

#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Camera/Camera.h>
#include <Graphics/Renderer/Renderer.h>
#include <PhysX/PhysXManager.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Sound/SoundManager.h>

#include <Graphics/Camera/FPSCameraController.h>

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <Core/Scene/SceneManager.h>

#include <Core/Component/ComponentManager.h>

#include <XML/XMLTreeNode.h>



CApplication::CApplication(CContextManager *_ContextManager)
	: m_ContextManager(_ContextManager)
	, m_FixedTimer(0)
	, m_Timer(0)
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

	engine.getSceneManager()->Update();

	CEngine::GetSingleton().getComponentManager()->PhysxUpdate();

	CEngine::GetSingleton().getCinematicsActionManager()->Update();
	CEngine::GetSingleton().getCinematicManager()->Update(_ElapsedTime);
	CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CCamera l_Camera = { };
	CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController()->UpdateCameraValues(&l_Camera);
	CEngine::GetSingleton().getSoundManager()->Update(&l_Camera);


	if (CInputManager::GetInputManager()->IsActionActive("FIXCAMERA"))
	{
		m_FixedCamera = !m_FixedCamera;
	}

	m_FixedTimer += _ElapsedTime;

	if ( m_FixedTimer >= PHYSX_UPDATE_STEP )
	{
		m_FixedTimer = fmod(m_FixedTimer, PHYSX_UPDATE_STEP);
		CEngine::GetSingleton().getPhysXManager()->update( PHYSX_UPDATE_STEP );
		CEngine::GetSingleton().getComponentManager()->FixedUpdate( PHYSX_UPDATE_STEP );
		CEngine::GetSingleton().getSceneManager()->FixedUpdate();
	}

	CEngine::GetSingleton().getComponentManager()->Update(_ElapsedTime);

	CEngine::GetSingleton().getCameraManager()->Update(_ElapsedTime);
}


void CApplication::Render()
{
	auto renderer = CEngine::GetSingleton().getRenderer();

	renderer->BeginRender();

	CEngine::GetSingleton().getComponentManager()->Render(*m_ContextManager);

	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);

	renderer->EndRender();
}
