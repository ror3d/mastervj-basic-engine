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
	owner->SetYaw( cc->GetYaw() );
}

void CFPSCameraComponent::Destroy()
{
	CEngine::GetSingleton().getCameraManager()->remove( getName() );
}


void CFPSCameraComponent::SetAsCurrentCamera()
{
	CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController( getName() );
}
