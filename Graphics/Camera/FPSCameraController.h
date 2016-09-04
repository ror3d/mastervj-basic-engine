#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "YawPitchCameraController.h"

class CCamera;

class CFPSCameraController : public IYawPitchCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	Vect3f						m_CameraOffset;
	float						m_PitchDisplacement;
	float						m_PitchfloorLimit;
	float						m_PitchSkyLimit;

	Vect3f m_TargetPosition;
public:
	CFPSCameraController();
	CFPSCameraController(const Vect3f& position, const Vect3f& offset, float YawSpeed, float PitchSpeed, float PitchFloorLimit, float PitchSkyLimit);
	CFPSCameraController(const Vect3f& cameraOffset, float YawSpeed, float PitchSpeed, float PitchDisplacement, float PitchFloorLimit, float PitchSkyLimit);
	virtual ~CFPSCameraController();

	virtual std::string GetType() { return "FPSCameraController"; }

	void UpdateCameraValues(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);

	inline void SetTargetPosition( const Vect3f &pos ) { m_TargetPosition = pos; }
	inline Vect3f GetTargetPosition() const { return m_TargetPosition; }

	inline void SetCameraOffset(const Vect3f &pos) { m_CameraOffset = pos; }

	Vect3f GetDirection() const;
	Vect3f GetCameraOffset() { return m_CameraOffset; }
	float GetPitchDisplacement(){ return m_PitchDisplacement; }

	void Update( float ElapsedTime );

};

#endif
