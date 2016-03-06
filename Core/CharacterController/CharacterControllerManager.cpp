#include "CharacterControllerManager.h"

#include "CharacterControllerInstance.h"
#include <Core/Engine/Engine.h>
#include <Camera/FPSCameraController.h>
#include <Renderable/RenderableObject.h>
#include <Renderable/RenderableObjectsManager.h>

CCharacterControllerManager::CCharacterControllerManager()
{
}

CCharacterControllerManager::~CCharacterControllerManager()
{
}

void CCharacterControllerManager::Create(std::string Name, std::string CameraName, std::string layerName, std::string modelName)
{
	ICameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CFPSCameraController* fpsCamera = dynamic_cast<CFPSCameraController*>(cc);
	DEBUG_ASSERT(fpsCamera != nullptr);
	
	CRenderableObjectsManager * layer = CEngine::GetSingleton().getLayerManager()->get(layerName);
	DEBUG_ASSERT(layer != nullptr);
	CRenderableObject * character = layer->get(modelName);
	DEBUG_ASSERT(character != nullptr);
	
	add(Name, new CCharacterControllerInstance(Name, fpsCamera, character));
}

void CCharacterControllerManager::UpdateInstances(float _elapsedTime)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); it++){
		it->second->Update(_elapsedTime);
	}
}