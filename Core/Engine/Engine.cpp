#include "Engine.h"

#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/CookedMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Light/LightManager.h>
#include <Graphics/Animation/AnimatedModelManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Graphics/Debug/DebugRender.h>
#include <Graphics/Camera/CameraManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/IA/IAManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Core/CharacterController/CharacterControllerManager.h>
#include <Core/Time/TimeManager.h>
#include <Core/Component/ComponentManager.h>
#include <Graphics/Particles/ParticleSystemManager.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Core/Trigger/TriggerManager.h>
#include <Core/Logic/LogicManager.h>

#ifndef CHECKED_DESTROY
#define CHECKED_DESTROY(x) do{ if(x) { x->destroy(); delete x; x = nullptr; } } while(0)
#endif

CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, layerManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
	, lightManager(nullptr)
	, debugRender(nullptr)
	, animatedModelManager(nullptr)
	, renderableObjectTechniqueManager(nullptr)
	, sceneRendererCommandManager(nullptr)
	, cameraManager(nullptr)
	, physXManager(nullptr)
	, cookedMeshManager(nullptr)
	, iaManager(nullptr)
	, scriptManager(nullptr)
	, characterControllerManager(nullptr)
	, timerManager(nullptr)
	, componentManager(nullptr)
	, cinematicsActionManager(nullptr)
	, cinematicManager(nullptr)
	, particleManager(nullptr)
	, triggerManager(nullptr)
	, logicManager(nullptr)
{
} 
CEngine::~CEngine()
{

	CHECKED_DESTROY(effectsManager);
	CHECKED_DESTROY(textureManager);
	CHECKED_DESTROY(materialManager);
	CHECKED_DESTROY(layerManager);
	CHECKED_DESTROY(staticMeshManager);
	CHECKED_DESTROY(lightManager);
	CHECKED_DESTROY(animatedModelManager);
	CHECKED_DESTROY(renderableObjectTechniqueManager);
	CHECKED_DESTROY(sceneRendererCommandManager);
	CHECKED_DESTROY(physXManager);
	CHECKED_DESTROY(cameraManager);
	CHECKED_DESTROY(contextManager);
	CHECKED_DESTROY(cookedMeshManager);
	CHECKED_DESTROY(iaManager);
	CHECKED_DESTROY(debugRender);
	CHECKED_DESTROY(scriptManager);
	CHECKED_DESTROY(characterControllerManager);
	CHECKED_DESTROY(timerManager);
	CHECKED_DESTROY(componentManager);
	CHECKED_DESTROY(particleManager);
	CHECKED_DESTROY(cinematicManager);
	CHECKED_DESTROY(cinematicsActionManager);
	CHECKED_DESTROY(triggerManager);
	CHECKED_DESTROY(logicManager);

} //Destruimos vars

void CEngine::Init()
{
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();	
	layerManager = new CLayerManager();
	staticMeshManager = new CStaticMeshManager();
	contextManager = new CContextManager();
	debugRender = new CDebugRender();
	lightManager = new CLightManager();
	animatedModelManager = new CAnimatedModelManager();
	renderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	sceneRendererCommandManager = new CSceneRendererCommandManager();
	cameraManager = new CCameraManager(contextManager);
	physXManager = CPhysXManager::CreatePhysXManager();
	cookedMeshManager = new CCookedMeshManager();
	iaManager = new CIAManager();
	scriptManager = new CScriptManager();
	characterControllerManager = new CCharacterControllerManager();
	timerManager = new CTimerManager();
	componentManager = new CComponentManager();
	particleManager = new CParticleSystemManager();
	cinematicsActionManager = new CCinematicsActionManager();
	cinematicManager = new CCinematicManager();
	triggerManager = new CTriggerManager();
	logicManager = new CLogicManager();
}
