#include "IAInstance.h"

#include "Engine/Engine.h"
#include "Renderable/RenderableObject.h"
#include "Renderable/RenderableObjectsManager.h"


CIAInstance::CIAInstance(std::string Name, float timeToUp) : CNamed(Name)
{
	m_IAOb = CEngine::GetSingleton().getLayerManager()->get("IA")->get(Name);
	timeToUpdate = timeToUp;
}

CIAInstance::~CIAInstance()
{

}

void CIAInstance::Update()
{
	 
}