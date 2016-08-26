#pragma once

#include <Base/Math/Math.h>

class CXMLTreeNode;

class CCinematicObjectKeyFrame
{
private:
	float m_KeyFrameTime;

	Vect3f m_Position;
	Quatf m_quat;
	Vect3f m_Scale;
public:
	CCinematicObjectKeyFrame( CXMLTreeNode &XMLTreeNode );

	inline float getKeyFrameTime() const { return m_KeyFrameTime; }
	inline void setKeyFrameTime( float kft ) { m_KeyFrameTime = kft; }

	inline Quatf GetQuat() const { return m_quat; }
	inline Vect3f GetPosition() const { return m_Position; }
	inline Vect3f GetScale() const { return m_Scale; }
};
