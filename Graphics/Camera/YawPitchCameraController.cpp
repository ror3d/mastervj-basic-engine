#include "YawPitchCameraController.h"

#include <cmath>

#include "Utils/Utils.h"

IYawPitchCameraController::IYawPitchCameraController()
: m_Yaw(0.0f)
, m_Pitch(0.0f)
{
}

IYawPitchCameraController::~IYawPitchCameraController()
{
}

Vect3f IYawPitchCameraController::GetRight() const
{
	return Vect3f(cos(m_Yaw-(FLOAT_PI_VALUE * 0.5f)), 0.0f, sin(m_Yaw-(FLOAT_PI_VALUE * 0.5f)));
}

Vect3f IYawPitchCameraController::GetUp() const
{
	Vect3f l_Up(-cos(m_Yaw)*sin(m_Pitch), cos(m_Pitch), -sin(m_Yaw)*sin(m_Pitch));
	return l_Up;
}

void IYawPitchCameraController::AddYaw(float Radians)
{
	m_Yaw+=Radians;
	if(m_Yaw>2*DEG2RAD(180.0f))
		m_Yaw-=2*DEG2RAD(180.0f);
	else if(m_Yaw<-2*DEG2RAD(180.0f))
		m_Yaw+=2*DEG2RAD(180.0f);
}
void IYawPitchCameraController::AddPitch(float Radians)
{
	if(((m_Pitch-Radians)<(DEG2RAD(180.0f)*0.5) && (m_Pitch-Radians)>-(DEG2RAD(180.0f)*0.5)))
	{
		m_Pitch-=Radians;
	}
}
