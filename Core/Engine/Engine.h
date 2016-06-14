#pragma once

#include <Base/Utils/SingletonPattern.h>

class CStaticMeshManager;
class CMaterialManager;
class CEffectManager;
class CTextureManager;
class CLightManager;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CContextManager;
class CDebugRender;
class CCameraManager;
class CPhysXManager;
class CCookedMeshManager;
class CScriptManager;
class CTimerManager;
class CComponentManager;
class CParticleSystemManager;
class CCinematicsActionManager;
class CCinematicManager;
class CTriggerManager;
class CSoundManager;
class CLogicManager;
class CStaticMeshLoader;
class CAnimatedMeshManager;
class CRenderer;

class CEngine : public CSingleton<CEngine>
{
private:
	CStaticMeshManager * staticMeshManager;
	CMaterialManager * materialManager;
	CEffectManager * effectsManager;
	CTextureManager * textureManager;
	CLightManager * lightManager;
	CRenderableObjectTechniqueManager * renderableObjectTechniqueManager;
	CSceneRendererCommandManager * sceneRendererCommandManager;
	CContextManager * contextManager;
	CDebugRender * debugRender;
	CCameraManager * cameraManager;
	CPhysXManager * physXManager;
	CCookedMeshManager * cookedMeshManager;
	CScriptManager * scriptManager;
	CTimerManager * timerManager;
	CComponentManager * componentManager;
	CParticleSystemManager * particleManager;
	CCinematicsActionManager * cinematicsActionManager;
	CCinematicManager * cinematicManager;
	CTriggerManager * triggerManager;
	CSoundManager * soundManager;
	CLogicManager * logicManager;
	CStaticMeshLoader * meshLoader;
	CAnimatedMeshManager * animatedMeshManager;
	CRenderer * renderer;

public:
	CEngine(); //Construimos vars y hacemos gets
	virtual ~CEngine(); //Destruimos vars

	void Init();

	//GETS
	inline CStaticMeshManager * getStaticMeshManager() const { return staticMeshManager; }
	inline CMaterialManager * getMaterialManager() const { return materialManager; }
	inline CEffectManager * getEffectsManager() const { return effectsManager; }
	inline CTextureManager * getTextureManager() const { return textureManager; }
	inline CContextManager * getContextManager() const { return contextManager; }
	inline CLightManager * getLightManager() const { return lightManager; }
	inline CRenderableObjectTechniqueManager * getRenderableObjectTechniqueManager() const { return renderableObjectTechniqueManager; }
	inline CSceneRendererCommandManager * getSceneRendererCommandManager() const { return sceneRendererCommandManager; }
	inline CDebugRender * getDebugRender() const { return debugRender; }
	inline CCameraManager * getCameraManager() const { return cameraManager; }
	inline CPhysXManager * getPhysXManager() const { return physXManager; }
	inline CCookedMeshManager * getCookedMeshManager() const { return cookedMeshManager;  }
	inline CScriptManager * getScriptManager() const { return scriptManager; }
	inline CTimerManager * getTimerManager() const { return timerManager;  }
	inline CComponentManager * getComponentManager() const { return componentManager;  }
	inline CParticleSystemManager * getParticleManager() const { return particleManager; }
	inline CCinematicsActionManager * getCinematicsActionManager() const { return cinematicsActionManager;  }
	inline CCinematicManager * getCinematicManager() const { return cinematicManager;  }
	inline CTriggerManager * getTriggerManager() const { return triggerManager;  }
	inline CSoundManager * getSoundManager() const { return soundManager; }
	inline CLogicManager * getLogicManager() const { return logicManager;  }
	inline CStaticMeshLoader * getMeshLoader() const { return meshLoader;  }
	inline CAnimatedMeshManager * getAnimatedMeshManager() const { return animatedMeshManager; }
	inline CRenderer * getRenderer() const { return renderer; }
};
