#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "CameraController.h"

class CCamera;

class CFPSCameraController : public CCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_Speed;
	float						m_FastSpeed;
	Vect3f						m_CameraDisplacement;
	float						m_PitchDisplacement;
public:
	CFPSCameraController();
	virtual ~CFPSCameraController();
	
	void Move(float Strafe, float Forward, bool Speed, float ElapsedTime);
	void SetCamera(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);
	Vect3f GetDirection() const;
	Vect3f GetCameraDisplacement() { return m_CameraDisplacement; }
	float GetPitchDisplacement(){ return m_PitchDisplacement; }

};

#endif
