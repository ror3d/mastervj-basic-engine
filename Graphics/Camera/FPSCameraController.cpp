#include "FPSCameraController.h"
#include "Camera.h"

#include <Core/Input/InputManager.h>

#include "Utils/Utils.h"

CFPSCameraController::CFPSCameraController()
: m_YawSpeed(0.08f)
, m_PitchSpeed(0.5f)
, m_CameraDisplacement(0, 0, -2)//(0,1.2f,-2)
, m_PitchDisplacement(0)//(-0.5f)
, m_PitchfloorLimit(DEG2RAD(60))
, m_PitchSkyLimit(DEG2RAD(-60))
{
}

CFPSCameraController::CFPSCameraController( const Vect3f& cameraOffset,
											float YawSpeed,
											float PitchSpeed,
											float PitchDisplacement,
											float PitchFloorLimit,
											float PitchSkyLimit )
	: m_CameraDisplacement(cameraOffset)
	, m_YawSpeed(YawSpeed)
	, m_PitchSpeed(PitchSpeed)
	, m_PitchDisplacement(PitchDisplacement)
	, m_PitchfloorLimit(PitchFloorLimit)
	, m_PitchSkyLimit(PitchSkyLimit)
{

}

CFPSCameraController::~CFPSCameraController()
{
}

void CFPSCameraController::AddYaw(float Radians)
{
	IYawPitchCameraController::AddYaw(Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{
	IYawPitchCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CFPSCameraController::UpdateCameraValues(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position + l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f CFPSCameraController::GetDirection() const
{
	Vect3f l_Direction(-sin(m_Yaw)*cos(m_Pitch), sin(m_Pitch), cos(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}

void CFPSCameraController::Update( float ElapsedTime )
{
	m_Pitch = m_Pitch - m_PitchDisplacement;

	AddYaw(CInputManager::GetInputManager()->GetAxis("X_AXIS"));
	AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS"));
	if (m_Pitch > m_PitchfloorLimit)//No atraviesa suelo
	{
		m_Pitch = m_PitchfloorLimit;
	}
	if (m_Pitch < m_PitchSkyLimit)//Vista superior personaje
	{
		m_Pitch = m_PitchSkyLimit;
	}

	//IF CHARCONTROLLER
	//receive updated pos

	//Vect3f rotacionDeseada = m_CameraDisplacement;
	//rotacionDeseada = rotacionDeseada.RotateY(-m_Yaw);
	//rotacionDeseada = rotacionDeseada.RotateX(m_Pitch);
	Mat33f rot;
	rot.SetIdentity();
	rot.RotByAngleX( -m_Pitch );
	rot.RotByAngleY( -m_Yaw );

	m_Position = m_TargetPosition + rot * m_CameraDisplacement;
	m_Pitch = m_Pitch + m_PitchDisplacement;
}
