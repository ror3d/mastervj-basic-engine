#include "Engine.h"

#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/StaticMeshLoader.h>
#include <Graphics/Mesh/CookedMeshManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Light/LightManager.h>
#include <Graphics/Animation/AnimatedMeshManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Graphics/Debug/DebugRender.h>
#include <Graphics/Camera/CameraManager.h>
#include <PhysX/PhysXManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Core/Time/TimeManager.h>
#include <Core/Component/ComponentManager.h>
#include <Graphics/Particles/ParticleSystemManager.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Core/Trigger/TriggerManager.h>
#include <Sound/SoundManager.h>
#include <Core/Logic/LogicManager.h>
#include <Graphics/Renderer/Renderer.h>
#include <Core/Scene/SceneManager.h>

#ifndef CHECKED_DESTROY
#define CHECKED_DESTROY(x) do{ if(x) { x->destroy(); delete x; x = nullptr; } } while(0)
#endif

CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
	, lightManager(nullptr)
	, debugRender(nullptr)
	, renderableObjectTechniqueManager(nullptr)
	, sceneRendererCommandManager(nullptr)
	, cameraManager(nullptr)
	, physXManager(nullptr)
	, cookedMeshManager(nullptr)
	, scriptManager(nullptr)
	, timerManager(nullptr)
	, componentManager(nullptr)
	, cinematicsActionManager(nullptr)
	, cinematicManager(nullptr)
	, particleManager(nullptr)
	, triggerManager(nullptr)
	, soundManager(nullptr)
	, logicManager(nullptr)
	, meshLoader(nullptr)
	, animatedMeshManager(nullptr)
	, renderer(nullptr)
	, sceneManager(nullptr)
{
}
CEngine::~CEngine()
{
	CHECKED_DESTROY(componentManager);
	CHECKED_DESTROY(scriptManager);
	CHECKED_DESTROY(effectsManager);
	CHECKED_DESTROY(textureManager);
	CHECKED_DESTROY(materialManager);
	CHECKED_DESTROY(staticMeshManager);
	CHECKED_DESTROY(lightManager);
	CHECKED_DESTROY(renderableObjectTechniqueManager);
	CHECKED_DESTROY(sceneRendererCommandManager);
	CHECKED_DESTROY(physXManager);
	CHECKED_DESTROY(cameraManager);
	CHECKED_DESTROY(contextManager);
	CHECKED_DESTROY(cookedMeshManager);
	CHECKED_DESTROY(debugRender);
	CHECKED_DESTROY(timerManager);
	CHECKED_DESTROY(particleManager);
	CHECKED_DESTROY(cinematicManager);
	CHECKED_DESTROY(cinematicsActionManager);
	CHECKED_DESTROY(triggerManager);
	CHECKED_DESTROY(soundManager);
	CHECKED_DESTROY(logicManager);
	CHECKED_DESTROY(meshLoader);
	CHECKED_DESTROY(animatedMeshManager);
	CHECKED_DESTROY(renderer);
	CHECKED_DESTROY(sceneManager);

} //Destruimos vars

void CEngine::Init()
{
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();
	staticMeshManager = new CStaticMeshManager();
	contextManager = new CContextManager();
	debugRender = new CDebugRender();
	lightManager = new CLightManager();
	renderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	sceneRendererCommandManager = new CSceneRendererCommandManager();
	cameraManager = new CCameraManager(contextManager);
	physXManager = CPhysXManager::CreatePhysXManager();
	cookedMeshManager = new CCookedMeshManager();
	scriptManager = new CScriptManager();
	timerManager = new CTimerManager();
	componentManager = new CComponentManager();
	particleManager = new CParticleSystemManager();
	cinematicsActionManager = new CCinematicsActionManager();
	cinematicManager = new CCinematicManager();
	triggerManager = new CTriggerManager();
	soundManager = new CSoundManager();
	logicManager = new CLogicManager();
	meshLoader = new CStaticMeshLoader();
	animatedMeshManager = new CAnimatedMeshManager();
	renderer = new CRenderer();
	sceneManager = new CSceneManager();
}
