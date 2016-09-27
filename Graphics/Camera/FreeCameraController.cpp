#include "FreeCameraController.h"
#include "Camera.h"

CFreeCameraController::CFreeCameraController()
	: m_forward(1, 0, 0)
	, m_up(0, 1, 0)
	, m_offset(0, 0, 0)
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
	Camera->SetMatrixs();
}

void CFreeCameraController::Update( float ElapsedTime )
{
}
