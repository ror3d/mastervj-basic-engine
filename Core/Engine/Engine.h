#pragma once

#include <Base/Utils/SingletonPattern.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/CookedMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Light/LightManager.h>
#include <Graphics/Animation/AnimatedModelManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Graphics/Debug/DebugRender.h>
#include <Graphics/Camera/CameraManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/IA/IAManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Core/CharacterController/CharacterControllerManager.h>
#include <Core/Time/TimeManager.h>
#include <Core/Component/ComponentManager.h>
#include <Graphics/Particles/ParticleSystemManager.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Core/Trigger/TriggerManager.h>


class CEngine : public CSingleton<CEngine>
{
private:
	CStaticMeshManager * staticMeshManager;
	CLayerManager * layerManager;
	CMaterialManager * materialManager;
	CEffectManager * effectsManager;
	CTextureManager * textureManager;
	CLightManager * lightManager;
	CAnimatedModelManager * animatedModelManager;
	CRenderableObjectTechniqueManager * renderableObjectTechniqueManager;
	CSceneRendererCommandManager * sceneRendererCommandManager;
	CContextManager * contextManager;
	CDebugRender * debugRender;
	CCameraManager * cameraManager;
	CPhysXManager * physXManager;
	CCookedMeshManager * cookedMeshManager;
	CIAManager * iaManager;
	CScriptManager * scriptManager;
	CCharacterControllerManager * characterControllerManager;
	CTimerManager * timerManager;
	CComponentManager * componentManager;
	CParticleSystemManager * particleManager;
	CCinematicsActionManager * cinematicsActionManager;
	CCinematicManager * cinematicManager;
	CTriggerManager * triggerManager;

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
	inline CContextManager * getContextManager() const { return contextManager; }
	inline CLightManager * getLightManager() const { return lightManager; }
	inline CAnimatedModelManager * getAnimatedModelManager() const { return animatedModelManager; }
	inline CRenderableObjectTechniqueManager * getRenderableObjectTechniqueManager() const { return renderableObjectTechniqueManager; }
	inline CSceneRendererCommandManager * getSceneRendererCommandManager() const { return sceneRendererCommandManager; }
	inline CDebugRender * getDebugRender() const { return debugRender; }
	inline CCameraManager * getCameraManager() const { return cameraManager; }
	inline CPhysXManager * getPhysXManager() const { return physXManager; }
	inline CCookedMeshManager * getCookedMeshManager() const { return cookedMeshManager;  }
	inline CIAManager * getIAManager() const { return iaManager; }
	inline CScriptManager * getScriptManager() const { return scriptManager; }
	inline CCharacterControllerManager * getCharacterControllerManager() const { return characterControllerManager; }
	inline CTimerManager * getTimerManager() const { return timerManager;  }
	inline CComponentManager * getComponentManager() const { return componentManager;  }
	inline CParticleSystemManager * getParticleManager() const { return particleManager; }
	inline CCinematicsActionManager * getCinematicsActionManager() const { return cinematicsActionManager;  }
	inline CCinematicManager * getCinematicManager() const { return cinematicManager;  }
	inline CTriggerManager * getTriggerManager() const { return triggerManager;  }
};
