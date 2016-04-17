#include "Engine.h"

#include "Context/ContextManager.h"


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
{
} 
CEngine::~CEngine()
{
	effectsManager->destroy();
	textureManager->destroy();
	materialManager->destroy();
	layerManager->destroy();
	staticMeshManager->destroy();
	lightManager->destroy();
	animatedModelManager->destroy();
	renderableObjectTechniqueManager->destroy();
	sceneRendererCommandManager->destroy();
	cameraManager->destroy();
	contextManager->destroy();
	scriptManager->Destroy();
	characterControllerManager->destroy();
	componentManager->destroy();
	particleManager->destroy();
	cinematicManager->destroy();
	triggerManager->destroy();
	//cinematicsActionManager->destroy();
	//debugRender->destroy();
	//cookedMeshManager->destroy();
	//iaManager->destroy();

	delete effectsManager;
	delete textureManager;
	delete materialManager;
	delete layerManager;
	delete staticMeshManager;
	delete lightManager;
	delete animatedModelManager;
	delete renderableObjectTechniqueManager;
	delete sceneRendererCommandManager;
	delete physXManager;
	delete cameraManager;
	delete contextManager;
	delete cookedMeshManager;
	delete iaManager;
	delete debugRender;
	delete scriptManager;
	delete characterControllerManager;
	delete timerManager;
	delete componentManager;
	delete particleManager;
	delete cinematicsActionManager;
	delete cinematicManager;
	delete triggerManager;

	effectsManager = nullptr;
	textureManager = nullptr;
	materialManager = nullptr;
	layerManager = nullptr;
	staticMeshManager = nullptr,
	lightManager = nullptr;
	animatedModelManager = nullptr;
	renderableObjectTechniqueManager = nullptr;
	sceneRendererCommandManager = nullptr;
	physXManager = nullptr;
	cameraManager = nullptr;
	contextManager = nullptr;
	cookedMeshManager = nullptr;
	iaManager = nullptr;
	debugRender = nullptr;
	scriptManager = nullptr;
	characterControllerManager = nullptr;
	timerManager = nullptr;
	componentManager = nullptr;
	particleManager = nullptr;
	cinematicsActionManager = nullptr;
	cinematicManager = nullptr;
	triggerManager = nullptr;

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
}
