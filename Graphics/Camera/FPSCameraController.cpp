#include "FPSCameraController.h"
#include "Camera.h"

#include <Core/Input/InputManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

#include "Utils/Utils.h"
#include <Base/Math/Math.h>

CFPSCameraController::CFPSCameraController()
: m_YawSpeed(0.08f)
, m_PitchSpeed(0.5f)
, m_CameraDisplacement(0, 0, -10)
, m_PitchDisplacement(0)//(-0.5f)
, m_PitchfloorLimit(DEG2RAD(20))//60
, m_PitchSkyLimit(DEG2RAD(-30))
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
	if (m_Pitch > m_PitchfloorLimit)
	{
		m_Pitch = m_PitchfloorLimit;
	}
	if (m_Pitch < m_PitchSkyLimit)
	{
		m_Pitch = m_PitchSkyLimit;
	}

	Mat33f rot;
	rot.SetIdentity();
	rot.RotByAngleX( -m_Pitch );
	rot.RotByAngleY( -m_Yaw );

	Vect3f posWithoutCollision = m_TargetPosition + rot * m_CameraDisplacement;

	
	m_Position = CEngine::GetSingleton().getPhysXManager()->RayCast(m_TargetPosition, posWithoutCollision - m_TargetPosition, mathUtils::Abs(m_CameraDisplacement.z));
		
	if (m_Position == Vect3f())
	{
		m_Position = posWithoutCollision;
	}

	//Add offset to no be inside the walls
	m_Position = m_Position + ((m_TargetPosition - m_Position) * 0.08);

	m_Pitch = m_Pitch + m_PitchDisplacement;
}
