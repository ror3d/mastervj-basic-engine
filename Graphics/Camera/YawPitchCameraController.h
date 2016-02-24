#ifndef INC_YAWPITCHCAMERACONTROLLER_H_
#define INC_YAWPITCHCAMERACONTROLLER_H_

#include "CameraController.h"

class IYawPitchCameraController : public ICameraController
{
protected:
	float						m_Yaw;
	float						m_Pitch;
public:
	IYawPitchCameraController();
	virtual ~IYawPitchCameraController();

	virtual void AddYaw(float Radians);
	virtual void AddPitch(float Radians);
	void SetYaw(float Yaw) {m_Yaw=Yaw;}
	float GetYaw() const {return m_Yaw;}
	void SetPitch(float Pitch) {m_Pitch=Pitch;}
	float GetPitch() const {return m_Pitch;}

	virtual Vect3f GetRight() const;
	virtual Vect3f GetUp() const;
};

#endif