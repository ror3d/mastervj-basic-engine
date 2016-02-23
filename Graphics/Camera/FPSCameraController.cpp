#include "FPSCameraController.h"
#include "Camera.h"

#include <Core/Input/InputManager.h>

CFPSCameraController::CFPSCameraController()
: m_YawSpeed(100.f)
, m_PitchSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
{
	m_Position=Vect3f(0.0f, 2.0f, 0.0f);
}

CFPSCameraController::~CFPSCameraController()
{
}

void CFPSCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
{
	Vect3f l_AddPos;
	l_AddPos.y=0.0f;
	l_AddPos.x=Forward*(cos(m_Yaw))+Strafe*(cos(m_Yaw+3.14159f*0.5f));
	l_AddPos.z=Forward*(sin(m_Yaw))+Strafe*(sin(m_Yaw+3.14159f*0.5f));

	float l_ConstantSpeed=ElapsedTime*m_Speed;
	if(Speed)
		l_ConstantSpeed*=m_FastSpeed;

	if (l_AddPos.SquaredLength() > 0)
	{
		l_AddPos.Normalize();
		l_AddPos *= l_ConstantSpeed;
		m_Position += l_AddPos;
	}
}

void CFPSCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(-Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CFPSCameraController::UpdateCameraValues(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f CFPSCameraController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}

void CFPSCameraController::Update( float ElapsedTime )
{
	AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * 0.0005f);
	AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * 0.005f);
}
