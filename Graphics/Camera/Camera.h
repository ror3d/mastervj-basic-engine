#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "CameraInfo.h"

class CCamera : public CCameraInfo
{
private:
	Mat44f				m_View;
	Mat44f				m_Projection;


public:
	CCamera()
	{}
	virtual ~CCamera() {}


	const Mat44f & GetView() const { return m_View; }
	const Mat44f & GetProjection() const { return m_Projection; }

	void SetMatrixs()
	{
		m_View.SetIdentity();
		m_View.SetFromLookAt(GetPosition(), GetLookAt(), GetUp());

		m_Projection.SetIdentity();
		m_Projection.SetFromPerspective(GetFOV(), GetAspectRatio(), GetZNear(), GetZFar());
	}
};

#endif
