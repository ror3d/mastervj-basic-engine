#include "SphericalCameraController.h"
#include "Camera.h"
#include "Utils/Utils.h"

#include <Core/Input/InputManager.h>

#include <math.h>
#include <stdio.h>



CSphericalCameraController::CSphericalCameraController()
: m_Zoom(50.f)
, m_ZoomSpeed(2.f)
{
}

CSphericalCameraController::~CSphericalCameraController()
{
}

Vect3f CSphericalCameraController::GetDirection() const
{
	return Vect3f(-m_Zoom*sin(m_Yaw)*cos(m_Pitch),
		m_Zoom*sin( m_Pitch ),
		m_Zoom*cos( m_Yaw )*cos( m_Pitch ) );
}

void CSphericalCameraController::UpdateRotation( Vect3f movement )
{
	AddYaw(movement.x*30.0f);
	AddPitch(movement.y*30.0f);
	AddZoom(-movement.z*2.0f);
}

void CSphericalCameraController::Update( float ElapsedTime )
{
	if ( CInputManager::GetInputManager()->IsActionActive( "MOVE_CAMERA" ) )
	{
		Vect3f cameraMovement( 0, 0, 0 );

		cameraMovement.x = CInputManager::GetInputManager()->GetAxis( "X_AXIS" ) * 0.0005f;
		cameraMovement.y = CInputManager::GetInputManager()->GetAxis( "Y_AXIS" ) * 0.005f;
		UpdateRotation(cameraMovement);
	}

	float Strafe = CInputManager::GetInputManager()->GetAxis("STRAFE");
	float Forward = CInputManager::GetInputManager()->GetAxis("MOVE_FWD");

	Vect3f cameraPosition = Vect3f(0, 0, 0);
	cameraPosition.x = -Forward*sin(m_Yaw) - Strafe*cos(m_Yaw);
	cameraPosition.z = Forward*cos(m_Yaw) - Strafe*sin(m_Yaw);
	cameraPosition.y = CInputManager::GetInputManager()->GetAxis("JUMPAxis");
	m_Position += cameraPosition*0.1f;
}

void CSphericalCameraController::UpdateCameraValues(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();

	Camera->SetPosition(m_Position-l_Direction);

	Camera->SetLookAt(m_Position);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}
