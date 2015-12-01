#include "Engine.h"


CEngine::CEngine()
	: effectsManager(nullptr)
	, materialManager(nullptr)
	, renderableObjectManager(nullptr)
	, staticMeshManager(nullptr)
	, textureManager(nullptr)
{
} 
CEngine::~CEngine(){
	delete effectsManager;
	delete materialManager;
	delete renderableObjectManager;
	delete staticMeshManager;
	delete textureManager;
	effectsManager = NULL;
	materialManager = NULL;
	renderableObjectManager = NULL;
	staticMeshManager = NULL,
	textureManager = NULL;
} //Destruimos vars

void CEngine::Init(){
	effectsManager = new CEffectManager();
	textureManager = new CTextureManager();
	materialManager = new CMaterialManager();
	renderableObjectManager = new CRenderableObjectsManager();
	staticMeshManager = new CStaticMeshManager();
	renderManager = new CRenderManager();
}