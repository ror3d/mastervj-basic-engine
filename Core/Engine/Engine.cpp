#include "Engine.h"

#include "Context/ContextManager.h"


CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, renderableObjectManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
	, lightManager(nullptr)
	, animatedModelManager(nullptr)
	, sceneRendererCommandManager(nullptr)
{
} 
CEngine::~CEngine()
{
	effectsManager->destroy();
	textureManager->destroy();
	materialManager->destroy();
	renderableObjectManager->destroy();
	staticMeshManager->destroy();
	renderManager->destroy();
	contextManager->destroy();
	lightManager->destroy();
	animatedModelManager->destroy();
	sceneRendererCommandManager->destroy();

	delete effectsManager;
	delete textureManager;
	delete materialManager;
	delete renderableObjectManager;
	delete staticMeshManager;
	delete renderManager;
	delete contextManager;
	delete lightManager;
	delete animatedModelManager;
	delete sceneRendererCommandManager;

	effectsManager = nullptr;
	textureManager = nullptr;
	materialManager = nullptr;
	renderableObjectManager = nullptr;
	staticMeshManager = nullptr,
	renderManager = nullptr;
	contextManager = nullptr;
	lightManager = nullptr;
	animatedModelManager = nullptr;
	sceneRendererCommandManager = nullptr;

} //Destruimos vars

void CEngine::Init()
{
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();
	renderableObjectManager = new CRenderableObjectsManager();
	staticMeshManager = new CStaticMeshManager();
	renderManager = new CRenderManager();
	contextManager = new CContextManager();
	lightManager = new CLightManager();
	animatedModelManager = new CAnimatedModelManager();
	sceneRendererCommandManager = new CSceneRendererCommandManager();
}
