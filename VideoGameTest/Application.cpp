#include "Application.h"

#include <Base/Math/Math.h>

#include <Core/Engine/Engine.h>

#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Animation/AnimatedModelManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Camera/CameraManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/CharacterController/CharacterControllerManager.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Graphics/Cinematics/CinematicManager.h>


#include <Graphics/Camera/FPSCameraController.h>
#include "Animation/AnimatedInstanceModel.h"
#include "Animation/AnimatedCoreModel.h"

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

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

	CEngine::GetSingleton().getComponentManager()->Update(_ElapsedTime);
	CEngine::GetSingleton().getLayerManager()->Update(_ElapsedTime);
	CEngine::GetSingleton().getCinematicsActionManager()->Update();
	CEngine::GetSingleton().getCinematicManager()->Update(_ElapsedTime);


	if (CInputManager::GetInputManager()->IsActionActive("FIXCAMERA"))
	{
		m_FixedCamera = !m_FixedCamera;
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

	m_FixedTimer += _ElapsedTime;

	if ( m_FixedTimer >= PHYSX_UPDATE_STEP )
	{
		m_FixedTimer = fmod(m_FixedTimer, PHYSX_UPDATE_STEP);
		CEngine::GetSingleton().getComponentManager()->FixedUpdate( PHYSX_UPDATE_STEP );
		CEngine::GetSingleton().getPhysXManager()->update( PHYSX_UPDATE_STEP );
	}
}


void CApplication::Render()
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Execute(*m_ContextManager);
}
