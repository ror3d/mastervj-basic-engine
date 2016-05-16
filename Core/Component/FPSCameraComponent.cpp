#include "FPSCameraComponent.h"

#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Core/Engine/Engine.h>

CFPSCameraComponent::CFPSCameraComponent(CXMLTreeNode& node, CRenderableObject* Owner)
	: CComponent(node, Owner)
{
	setName(Owner->getName() + "_FPSCamera");

	m_CamOffset = node.GetVect3fProperty( "offset", Vect3f( 0, 0, 0 ), false );
}

CFPSCameraComponent::CFPSCameraComponent(CRenderableObject* Owner)
	: CComponent(Owner->getName() + "_FPSCamera", Owner)
{
}

CFPSCameraComponent::~CFPSCameraComponent()
{}

void CFPSCameraComponent::Init()
{
	CRenderableObject *owner = GetOwner();
	CFPSCameraController* cc = new CFPSCameraController();

	cc->SetPosition( owner->GetPosition() );
	cc->SetYaw( owner->GetYaw() );

	CEngine::GetSingleton().getCameraManager()->add(getName(), cc);
}

void CFPSCameraComponent::Update(float elapsedTime)
{
	CRenderableObject *owner = GetOwner();
	CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController( getName() );
	CFPSCameraController* cc = dynamic_cast<CFPSCameraController*>(CEngine::GetSingleton().getCameraManager()->get( getName() ));
	DEBUG_ASSERT( cc != nullptr );

	cc->SetTargetPosition( owner->GetPosition() + m_CamOffset );
	owner->SetYaw( cc->GetYaw() );
}

void CFPSCameraComponent::Destroy()
{
	CEngine::GetSingleton().getCameraManager()->remove( getName() );
}

