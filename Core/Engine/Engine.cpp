#include "Engine.h"

#include "Context/ContextManager.h"


CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, renderableObjectManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
	, lightManager(nullptr)
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

	delete effectsManager;
	delete textureManager;
	delete materialManager;
	delete renderableObjectManager;
	delete staticMeshManager;
	delete renderManager;
	delete contextManager;
	delete lightManager;
	effectsManager = nullptr;
	textureManager = nullptr;
	materialManager = nullptr;
	renderableObjectManager = nullptr;
	staticMeshManager = nullptr,
	renderManager = nullptr;
	contextManager = nullptr;
	lightManager = nullptr;
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
}
