#pragma once

#include <vector>
#include <string>

#include "Camera/CameraController.h"

class CXMLTreeNode;
class CCameraKey;
class CCamera;

class CCameraKeyController : public ICameraController
{
private:
	std::vector<CCameraKey *> m_Keys;
	std::string m_CameraName;
	size_t m_CurrentKey;
	size_t m_NextKey;
	float m_CurrentTime;
	float m_TotalTime;

	bool m_Cycle;
	bool m_Reverse;
	bool m_Playing;

	void GetCurrentKey();
public:
	CCameraKeyController( CXMLTreeNode &XMLTreeNode );
	~CCameraKeyController();

	void Update( float ElapsedTime );

	void Play(){ m_Playing = true; }

	void SetCurrentTime( float CurrentTime );
	void ResetTime();
	float GetTotalTime();

	void UpdateCameraValues(CCamera * cam) const;

	bool IsCycle() const { return m_Cycle; }
	void SetCycle( bool Cycle ) { m_Cycle = Cycle; }

	bool IsReverse() const { return m_Reverse; }
	void SetReverse( bool Reverse ) { m_Reverse = Reverse; }
};
