#include "IAInstance.h"

#include "Engine/Engine.h"
#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Camera/CameraController.h>
#include <Graphics/Layer/LayerManager.h>

#include <math.h>


CIAInstance::CIAInstance(std::string Name, float timeToUp) : CNamed(Name)
	, m_actualTime(0)
	, m_timeToUpdate(timeToUp)
{
	m_IAOb = CEngine::GetSingleton().getLayerManager()->get("IA")->get(Name);
	GetTarget();

	m_velToMove = 1;
	m_velToRotate = DEG2RAD(20);
}

CIAInstance::~CIAInstance()
{
}

void CIAInstance::UpdateTarget(){
	GetTarget();
}

void CIAInstance::Update(float elapsedTime)
{
	m_actualTime += elapsedTime;
	float step;
	if (m_IAOb->GetPosition().Distance(m_target)>1.5f)
	{
		float posWithVel = m_actualTime*m_velToMove;
		if (m_distToTarget > 0)
		{
			step = posWithVel / m_distToTarget;
			m_IAOb->SetPosition(m_lastPosition.Lerp(m_target, step));
		}
	}
	//Lerp rotation
	float rotWithVel = m_actualTime*m_velToRotate;
	step = rotWithVel / m_anglesToTarget;
	if (m_anglesToTarget>0 && step<1){
		Vect3f rotResult = m_lastRotation.Lerp(m_rotTarget, step);
		m_IAOb->SetYaw(rotResult.y);
	}
}

void CIAInstance::GetTarget()
{
	m_actualTime = 0;

	CRenderableObject * character = nullptr;
	CRenderableObjectsManager * layer = CEngine::GetSingleton().getLayerManager()->get("models");
	if (layer != nullptr)
	{
		//Position
		character = layer->get("main");
		m_target = character->GetPosition();
		m_distToTarget = m_IAOb->GetPosition().Distance(m_target);
		m_lastPosition = m_IAOb->GetPosition();

		//Rotation
		Mat44f dir = m_IAOb->GetTransform();
		dir.SetFromLookAt(m_lastPosition, m_target, Vect3f(0, 1, 0));
		m_rotTarget = Vect3f(0, -dir.GetPitchRollYaw().y, 0);
		m_lastRotation = Vect3f(0, m_IAOb->GetYaw(), 0);
		if (m_lastRotation.y >= m_rotTarget.y)
		{
			m_anglesToTarget = m_lastRotation.y - m_rotTarget.y;
		}
		else
		{
			m_anglesToTarget = m_rotTarget.y - m_lastRotation.y;
		}

	}
}

