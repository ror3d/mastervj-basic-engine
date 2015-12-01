#include <Utils/SingletonPattern.h>
#include <Renderer/StaticMeshManager.h>
#include <Renderer/RenderableObjectsManager.h>
#include <Material/MaterialManager.h>
#include <Material/EffectManager.h>
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
	CStaticMeshManager * getStaticMeshManager(){ return staticMeshManager;}
	CRenderableObjectsManager * getRenderableObjectManager(){ return renderableObjectManager;}
	CMaterialManager * getMaterialManager(){ return materialManager;}
	CEffectManager * getEffectsManager(){ return effectsManager;}
	CTextureManager * getTextureManager(){ return textureManager;}	
	CRenderManager * getRenderManager(){ return renderManager;}	
	CContextManager * getContextManager(){ return contextManager;}	
};