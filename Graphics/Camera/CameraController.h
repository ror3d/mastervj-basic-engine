#ifndef INC_CAMERACONTROLLER_H_
#define INC_CAMERACONTROLLER_H_

#include <Base/Math/Math.h>

class CCamera;

class ICameraController
{
protected:
	Vect3f 				m_Position;
public:
	ICameraController();
	virtual ~ICameraController();
	virtual void UpdateCameraValues(CCamera *Camera) const = 0;

	const Vect3f & GetPosition() const { return m_Position; }
	void SetPosition(const Vect3f &Position) { m_Position = Position; }

	virtual void Update( float ElapsedTime ) = 0;
};

#endif
