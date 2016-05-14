#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "YawPitchCameraController.h"

class CCamera;

class CFPSCameraController : public IYawPitchCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_Speed;
	float						m_FastSpeed;
	Vect3f						m_CameraDisplacement;
	float						m_PitchDisplacement;
	float						m_PitchfloorLimit;
	float						m_PitchSkyLimit;
public:
	CFPSCameraController();
	virtual ~CFPSCameraController();

	virtual std::string GetType() { return "FPSCameraController"; }

	void Move(float Strafe, float Forward, bool Speed, float ElapsedTime);
	void UpdateCameraValues(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);
	Vect3f GetDirection() const;
	Vect3f GetCameraDisplacement() { return m_CameraDisplacement; }
	float GetPitchDisplacement(){ return m_PitchDisplacement; }

	void Update( float ElapsedTime );

};

#endif
