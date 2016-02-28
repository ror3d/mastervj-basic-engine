#pragma once

#include <Base/Math/Math.h>

class CXMLTreeNode;

class CCameraInfo
{
	Vect3f	m_Position;
	Vect3f	m_LookAt;
	Vect3f	m_Up;
	float	m_FOV;
	float	m_AspectRatio;
	float	m_ZNear;
	float	m_ZFar;
public:

	CCameraInfo();
	CCameraInfo( const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float
		NearPlane, float FarPlane, float FOV, float AspectRatio = 1.0f );
	CCameraInfo( CXMLTreeNode &XMLTreeNode );

	void SetFOV(float FOV) {m_FOV=FOV;}
	float GetFOV() const {return m_FOV;}
	void SetAspectRatio(float AspectRatio) {m_AspectRatio=AspectRatio;}
	float GetAspectRatio() const {return m_AspectRatio;}
	void SetZNear(float ZNear) {m_ZNear=ZNear;}
	float GetZNear() const {return m_ZNear;}
	void SetZFar(float ZFar) {m_ZFar=ZFar;}
	float GetZFar() const {return m_ZFar;}

	void SetPosition(const Vect3f &Position)
	{
		m_Position=Position;
	}
	const Vect3f & GetPosition() const
	{
		return m_Position;
	}
	void SetLookAt(const Vect3f &LookAt)
	{
		m_LookAt=LookAt;
	}
	const Vect3f & GetLookAt() const
	{
		return m_LookAt;
	}
	void SetUp(const Vect3f &Up)
	{
		m_Up=Up;
	}
	const Vect3f & GetUp() const
	{
		return m_Up;
	}
};
