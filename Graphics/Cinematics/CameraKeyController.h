#pragma once

#include <vector>
#include <string>

class CXMLTreeNode;
class CCameraKey;
class CCamera;

class CCameraKeyController
{
private:
	std::vector<CCameraKey *> m_Keys;
	size_t m_CurrentKey;
	size_t m_NextKey;
	float m_CurrentTime;
	float m_TotalTime;

	bool m_Cycle;
	bool m_Reverse;

	void GetCurrentKey();
public:
	CCameraKeyController( CXMLTreeNode &XMLTreeNode );
	~CCameraKeyController();

	void Update( float ElapsedTime );

	void SetCurrentTime( float CurrentTime );
	void ResetTime();
	float GetTotalTime();

	void AnimateCamera( CCamera* cam ) const;

	bool IsCycle() const { return m_Cycle; }
	void SetCycle( bool Cycle ) { m_Cycle = Cycle; }

	bool IsReverse() const { return m_Reverse; }
	void SetReverse( bool Reverse ) { m_Reverse = Reverse; }
};
