#include "FreeCameraComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Camera/FreeCameraController.h>
#include <Core/Engine/Engine.h>

const std::string CFreeCameraComponent::COMPONENT_TYPE = "FreeCamera";

CFreeCameraComponent::CFreeCameraComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_camController = new CFreeCameraController();

	Vect3f fwd = node.GetVect3fProperty("forward", Vect3f( 1, 0, 0), false);
	Vect3f off = node.GetVect3fProperty( "offset", Vect3f( 0, 0, 0 ), false );
	float fov = node.GetFloatProperty( "fov", -1, false );

	m_camController->SetForward( fwd );
	m_camController->SetOffset( off );

	if ( fov > 0 )
	{
		m_camController->SetFOV( mathUtils::Deg2Rad(fov) );
	}
}

CFreeCameraComponent::CFreeCameraComponent(const CFreeCameraComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );
}

CFreeCameraComponent::~CFreeCameraComponent()
{}

void CFreeCameraComponent::Init()
{
	CElement *owner = GetOwner();

	m_camController->SetPosition( owner->GetPosition() );

	CEngine::GetSingleton().getCameraManager()->add(getName(), m_camController);
}

void CFreeCameraComponent::Reset()
{
	CElement *owner = GetOwner();


	m_camController->SetPosition( owner->GetPosition() );
}

void CFreeCameraComponent::Update(float elapsedTime)
{
	if ( m_Destroyed ) return;
	CElement *owner = GetOwner();

	m_camController->SetPosition( owner->GetPosition() );
}

void CFreeCameraComponent::Destroy()
{
	if ( m_Destroyed ) return;
	m_Destroyed = true;
	delete CEngine::GetSingleton().getCameraManager()->get( getName() );
	CEngine::GetSingleton().getCameraManager()->remove( getName() );
	m_camController = nullptr;
}


void CFreeCameraComponent::SetAsCurrentCamera()
{
	if ( m_Destroyed ) return;
	CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController( getName() );
}

void CFreeCameraComponent::SetForward(Vect3f fwd)
{
	if ( m_Destroyed ) return;
	m_camController->SetForward(fwd);
}

Vect3f CFreeCameraComponent::GetForward()
{
	if ( m_Destroyed ) return 0;
	return m_camController->GetForward();
}

void CFreeCameraComponent::SetUp(Vect3f up)
{
	if ( m_Destroyed ) return;
	m_camController->SetUp(up);
}

Vect3f CFreeCameraComponent::GetUp()
{
	if ( m_Destroyed ) return 0;
	return m_camController->GetUp();
}

void CFreeCameraComponent::SetOffset(Vect3f off)
{
	if ( m_Destroyed ) return;
	m_camController->SetOffset(off);
}

Vect3f CFreeCameraComponent::GetOffset()
{
	if ( m_Destroyed ) return 0;
	return m_camController->GetOffset();
}
