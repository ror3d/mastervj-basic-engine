#pragma once

#include <Base/Utils/SingletonPattern.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Renderer/RenderManager.h>
#include <Graphics/Light/LightManager.h>
#include <Graphics/Animation/AnimatedModelManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>


class CEngine : public CSingleton<CEngine>
{
private:
	CStaticMeshManager * staticMeshManager;
	CLayerManager * layerManager;
	CMaterialManager * materialManager;
	CEffectManager * effectsManager;
	CTextureManager * textureManager;
	CRenderManager * renderManager;
	CLightManager * lightManager;
	CAnimatedModelManager * animatedModelManager;
	CSceneRendererCommandManager * sceneRendererCommandManager;
	CContextManager * contextManager;

public:
	CEngine(); //Construimos vars y hacemos gets
	virtual ~CEngine(); //Destruimos vars

	void Init();

	//GETS
	inline CStaticMeshManager * getStaticMeshManager() const { return staticMeshManager; }
	inline CLayerManager * getLayerManager() const { return layerManager; }
	inline CMaterialManager * getMaterialManager() const { return materialManager; }
	inline CEffectManager * getEffectsManager() const { return effectsManager; }
	inline CTextureManager * getTextureManager() const { return textureManager; }
	inline CRenderManager * getRenderManager() const { return renderManager; }
	inline CContextManager * getContextManager() const { return contextManager; }
	inline CLightManager * getLightManager() const { return lightManager; }
	inline CAnimatedModelManager * getAnimatedModelManager() const { return animatedModelManager; }
	inline CSceneRendererCommandManager * getSceneRendererCommandManager() const { return sceneRendererCommandManager; }
};
