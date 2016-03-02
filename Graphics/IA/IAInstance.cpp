#include "IAInstance.h"

#include "Engine/Engine.h"
#include "Renderable/RenderableObject.h"
#include "Renderable/RenderableObjectsManager.h"
#include "Camera/CameraController.h"


CIAInstance::CIAInstance(std::string Name, float timeToUp) : CNamed(Name)
	, m_actualTime(0)
	, m_timeToUpdate(timeToUp)
{
	m_IAOb = CEngine::GetSingleton().getLayerManager()->get("IA")->get(Name);
	GetTarget();
	m_timeToReachTarget = 2;
}

CIAInstance::~CIAInstance()
{

}

void CIAInstance::Update(float elapsedTime)
{
	m_actualTime += elapsedTime;
	float step = m_actualTime / m_timeToReachTarget;
	if (m_IAOb->GetPosition().Distance(m_target)>1.5f)
	{
		Vect3f actualPos = m_IAOb->GetPosition();
		actualPos = actualPos.Lerp(m_target, step);
		m_IAOb->SetPosition(actualPos);

		Mat44f dir = m_IAOb->GetTransform();
		dir.SetFromLookAt(m_IAOb->GetPosition(), m_target, Vect3f(0, 1, 0));
		m_IAOb->SetYaw(-dir.GetPitchRollYaw().y);
	}

	if (step >= 1.f)
	{
		RecalculateTarget();
	}
}

void CIAInstance::RecalculateTarget(){
	GetTarget();
	m_actualTime = 0;
}

void CIAInstance::GetTarget(){
	CRenderableObject * character = nullptr;
	CRenderableObjectsManager * layer = CEngine::GetSingleton().getLayerManager()->get("models");
	if (layer != nullptr)
	{
		character = layer->get("main");
		m_target=character->GetPosition();
	}
}
