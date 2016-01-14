#pragma once

#include "Renderer/3DElement.h"

class CXMLTreeNode;

class CCinematicObjectKeyFrame : public C3DElement
{
private:
	float m_KeyFrameTime;
public:
	CCinematicObjectKeyFrame( CXMLTreeNode &XMLTreeNode );

	inline float getKeyFrameTime() const { return m_KeyFrameTime; }
	inline void setKeyFrameTime( float kft ) { m_KeyFrameTime = kft; }

	inline virtual void Render( CContextManager *_context ) {}
};
