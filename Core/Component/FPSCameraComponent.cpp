#include "FPSCameraComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Core/Engine/Engine.h>

const std::string CFPSCameraComponent::COMPONENT_TYPE = "Camera";

CFPSCameraComponent::CFPSCameraComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_CamOffset = node.GetVect3fProperty( "offset", Vect3f( 0, 5, 5 ), false );

	m_YawSpeed = node.GetFloatProperty("yaw_speed", 0.08f, false);
	m_PitchSpeed = node.GetFloatProperty("pitch_speed", 0.5f, false);
	m_PitchFloorLimit = DEG2RAD(node.GetFloatProperty("floor_limit", 90, false));
	m_PitchSkyLimit = - DEG2RAD(node.GetFloatProperty("sky_limit", 90, false));
}

CFPSCameraComponent::CFPSCameraComponent(const CFPSCameraComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_CamOffset = base.m_CamOffset;
}

CFPSCameraComponent::~CFPSCameraComponent()
{}

void CFPSCameraComponent::Init()
{
	CElement *owner = GetOwner();
	CFPSCameraController* cc = new CFPSCameraController(owner->GetPosition(), m_CamOffset, m_YawSpeed, m_PitchSpeed, m_PitchFloorLimit, m_PitchSkyLimit);

	cc->SetPosition( owner->GetPosition() );
	cc->SetYaw( owner->GetYaw() );

	CEngine::GetSingleton().getCameraManager()->add(getName(), cc);
}

void CFPSCameraComponent::Update(float elapsedTime)
{
	CElement *owner = GetOwner();
	CFPSCameraController* cc = dynamic_cast<CFPSCameraController*>(CEngine::GetSingleton().getCameraManager()->get( getName() ));
	DEBUG_ASSERT( cc != nullptr );

	cc->SetCameraOffset(m_CamOffset);
	cc->SetTargetPosition( owner->GetPosition() );
}

void CFPSCameraComponent::Destroy()
{
	delete CEngine::GetSingleton().getCameraManager()->get( getName() );
	CEngine::GetSingleton().getCameraManager()->remove( getName() );
}


void CFPSCameraComponent::SetAsCurrentCamera()
{
	CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController( getName() );
}

float CFPSCameraComponent::GetYaw(){
	CFPSCameraController* cc = dynamic_cast<CFPSCameraController*>(CEngine::GetSingleton().getCameraManager()->get(getName()));
	DEBUG_ASSERT(cc != nullptr);
	return cc->GetYaw();
}
