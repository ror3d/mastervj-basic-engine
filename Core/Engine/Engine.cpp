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
CEngine::~CEngine(){
	delete effectsManager;
	delete materialManager;
	delete renderableObjectManager;
	delete staticMeshManager;
	delete textureManager;
	delete lightManager;
	effectsManager = NULL;
	materialManager = NULL;
	renderableObjectManager = NULL;
	staticMeshManager = NULL,
	textureManager = NULL;
	lightManager = NULL;
} //Destruimos vars

void CEngine::Init(){
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();
	renderableObjectManager = new CRenderableObjectsManager();
	staticMeshManager = new CStaticMeshManager();
	renderManager = new CRenderManager();
	contextManager = new CContextManager();
	lightManager = new CLightManager();
}
