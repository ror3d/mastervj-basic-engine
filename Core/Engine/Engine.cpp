#include "Engine.h"

#include "Context/ContextManager.h"


CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, layerManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
	, lightManager(nullptr)
	, animatedModelManager(nullptr)
	, renderableObjectTechniqueManager(nullptr)
	, sceneRendererCommandManager(nullptr)
	, physicsManager(nullptr)
{
} 
CEngine::~CEngine()
{
	effectsManager->destroy();
	textureManager->destroy();
	materialManager->destroy();
	layerManager->destroy();
	staticMeshManager->destroy();
	renderManager->destroy();
	contextManager->destroy();
	lightManager->destroy();
	animatedModelManager->destroy();
	renderableObjectTechniqueManager->Destroy();
	sceneRendererCommandManager->destroy();
	//TODO: physicsManager->destroy();

	delete effectsManager;
	delete textureManager;
	delete materialManager;
	delete layerManager;
	delete staticMeshManager;
	delete renderManager;
	delete contextManager;
	delete lightManager;
	delete animatedModelManager;
	delete renderableObjectTechniqueManager;
	delete sceneRendererCommandManager;
	delete physicsManager;

	effectsManager = nullptr;
	textureManager = nullptr;
	materialManager = nullptr;
	layerManager = nullptr;
	staticMeshManager = nullptr,
	renderManager = nullptr;
	contextManager = nullptr;
	lightManager = nullptr;
	animatedModelManager = nullptr;
	renderableObjectTechniqueManager = nullptr;
	sceneRendererCommandManager = nullptr;
	physicsManager = nullptr;

} //Destruimos vars

void CEngine::Init()
{
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();
	layerManager = new CLayerManager();
	staticMeshManager = new CStaticMeshManager();
	renderManager = new CRenderManager();
	contextManager = new CContextManager();
	lightManager = new CLightManager();
	animatedModelManager = new CAnimatedModelManager();
	renderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	sceneRendererCommandManager = new CSceneRendererCommandManager();
	physicsManager = CPhysXManager::CreatePhysXManager();
}
