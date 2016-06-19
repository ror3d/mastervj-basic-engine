#include "FPSCameraComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Core/Engine/Engine.h>

CFPSCameraComponent::CFPSCameraComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);

	m_CamOffset = node.GetVect3fProperty( "offset", Vect3f( 0, 0, 0 ), false );
}

CFPSCameraComponent::CFPSCameraComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CFPSCameraComponent::~CFPSCameraComponent()
{}

void CFPSCameraComponent::Init()
{
	CElement *owner = GetOwner();
	CFPSCameraController* cc = new CFPSCameraController();

	cc->SetPosition( owner->GetPosition() );
	cc->SetYaw( owner->GetYaw() );

	CEngine::GetSingleton().getCameraManager()->add(getName(), cc);
}

void CFPSCameraComponent::Update(float elapsedTime)
{
	CElement *owner = GetOwner();
	CFPSCameraController* cc = dynamic_cast<CFPSCameraController*>(CEngine::GetSingleton().getCameraManager()->get( getName() ));
	DEBUG_ASSERT( cc != nullptr );

	cc->SetTargetPosition( owner->GetPosition() + m_CamOffset );
	if (m_followRenderableObject)
	{
		owner->SetYaw(cc->GetYaw() + m_characterRotationOverride);
	}
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

void CFPSCameraComponent::SetFollowCharacter(bool follow, float overrideRot)
{
	m_followRenderableObject = follow;
	m_characterRotationOverride = overrideRot;
}

float CFPSCameraComponent::GetYaw(){
	CFPSCameraController* cc = dynamic_cast<CFPSCameraController*>(CEngine::GetSingleton().getCameraManager()->get(getName()));
	DEBUG_ASSERT(cc != nullptr);
	return cc->GetYaw();
}
