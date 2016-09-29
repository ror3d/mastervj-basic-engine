#pragma once


#include "CameraController.h"

class CCamera;

class CFreeCameraController : public ICameraController
{
private:
	Vect3f m_forward;
	Vect3f m_up;
	Vect3f m_offset;
public:
	CFreeCameraController();
	virtual ~CFreeCameraController();

	virtual std::string GetType() { return "FreeCameraController"; }

	void UpdateCameraValues(CCamera *Camera) const;

	void Update( float ElapsedTime );

	inline void SetForward( Vect3f fwd ) { m_forward = fwd; }
	inline Vect3f GetForward() const { return m_forward; }

	inline void SetUp( Vect3f up ) { m_up = up; }
	inline Vect3f GetUp() const { return m_up; }

	inline void SetOffset( Vect3f offset ) { m_offset = offset; }
	inline Vect3f GetOffset() const { return m_offset; }
};


