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
, m_CameraOffset(0, 0, -10)
, m_PitchDisplacement(0)//(-0.5f)
, m_PitchfloorLimit(DEG2RAD(20))//60
, m_PitchSkyLimit(DEG2RAD(-30))
{
}

CFPSCameraController::CFPSCameraController (const Vect3f& position, const Vect3f& offset, float YawSpeed, float PitchSpeed, float PitchFloorLimit, float PitchSkyLimit)
: IYawPitchCameraController(position)
, m_YawSpeed(YawSpeed)
, m_PitchSpeed(PitchSpeed)
, m_CameraOffset(offset)
, m_PitchDisplacement(0)//(-0.5f)
, m_PitchfloorLimit(PitchFloorLimit)//60
, m_PitchSkyLimit(PitchSkyLimit)
{
}

CFPSCameraController::CFPSCameraController( const Vect3f& cameraOffset,
											float YawSpeed,
											float PitchSpeed,
											float PitchDisplacement,
											float PitchFloorLimit,
											float PitchSkyLimit )
	: m_CameraOffset(cameraOffset)
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
	if (m_CameraLocked)
		return;

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

	Vect3f offsettedPosition = m_TargetPosition;
	offsettedPosition.y += m_CameraOffset.y;


	Vect3f posWithoutCollision = m_TargetPosition + rot * Vect3f(0, 0, m_CameraOffset.z);

	Vect3f posWithCollision;

	bool hit = CEngine::GetSingleton().getPhysXManager()->RayCast(offsettedPosition, posWithoutCollision - offsettedPosition, mathUtils::Abs(m_CameraOffset.z), posWithCollision);

	if (hit)
	{
		m_Position = posWithCollision + ( offsettedPosition - posWithCollision ) * 0.05;
	}
	else
	{
		m_Position = posWithoutCollision;
	}
	//m_Position = posWithoutCollision;

	m_Pitch = m_Pitch + m_PitchDisplacement;
}
