#include "FreeCameraController.h"
#include "Camera.h"

CFreeCameraController::CFreeCameraController()
	: m_forward(1, 0, 0)
	, m_up(0, 1, 0)
	, m_offset(0, 0, 0)
	, m_fov(-1)
{
}

CFreeCameraController::~CFreeCameraController()
{
}

void CFreeCameraController::UpdateCameraValues( CCamera * Camera ) const
{
	Camera->SetPosition(m_Position + m_offset);
	Camera->SetLookAt(m_Position + m_offset + m_forward);
	Camera->SetUp(GetUp());
	if ( m_fov > 0 )
	{
		Camera->SetFOV( m_fov );
	}
	Camera->SetMatrixs();
}

void CFreeCameraController::Update( float ElapsedTime )
{
}
