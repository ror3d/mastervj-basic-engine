#pragma once

#include <Utils/SingletonPattern.h>
#include <Renderer/StaticMeshManager.h>
#include <Renderer/RenderableObjectsManager.h>
#include <Material/MaterialManager.h>
#include <Effect/EffectManager.h>
#include <Texture/TextureManager.h>
#include <Renderer/RenderManager.h>

class CContextManager;

class CEngine : public CSingleton<CEngine>
{
private:
	CStaticMeshManager * staticMeshManager;
	CRenderableObjectsManager * renderableObjectManager;
	CMaterialManager * materialManager;
	CEffectManager * effectsManager;
	CTextureManager * textureManager;
	CRenderManager * renderManager;
	CContextManager * contextManager;

public:
	CEngine(); //Construimos vars y hacemos gets
	virtual ~CEngine(); //Destruimos vars

	void Init();

	//GETS
	inline CStaticMeshManager * getStaticMeshManager() const { return staticMeshManager; }
	inline CRenderableObjectsManager * getRenderableObjectManager() const { return renderableObjectManager; }
	inline CMaterialManager * getMaterialManager() const { return materialManager; }
	inline CEffectManager * getEffectsManager() const { return effectsManager; }
	inline CTextureManager * getTextureManager() const { return textureManager; }
	inline CRenderManager * getRenderManager() const { return renderManager; }
	inline CContextManager * getContextManager() const { return contextManager; }
};
