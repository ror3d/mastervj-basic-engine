#ifndef INC_SPHERICALCAMERA_H_
#define INC_SPHERICALCAMERA_H_

#include "YawPitchCameraController.h"

class CSphericalCameraController : public IYawPitchCameraController
{
private:
	float						m_Zoom;
	float						m_ZoomSpeed;
public:
	CSphericalCameraController();
	virtual ~CSphericalCameraController();

	void AddZoom(float Zoom) {m_Zoom+=Zoom;}
	void SetZoom(float Zoom) {m_Zoom=Zoom;}

	void UpdateCameraValues(CCamera *Camera) const;
	Vect3f GetDirection() const;

	void UpdateRotation( Vect3f movement );

	void Update( float ElapsedTime );
};

#endif
