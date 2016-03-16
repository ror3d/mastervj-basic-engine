#ifndef CINEMATICS_ACTION_H
#define CINEMATICS_ACTION_H

#include "XML/XMLTreeNode.h"

class CCinematicsAction
{
public:
	float m_Time;

	CCinematicsAction(CXMLTreeNode node);
	virtual ~CCinematicsAction();

	float GetTime(){ return m_Time; }
	void SetTime(float time){ m_Time = time;}
	float GetDuration() { return 0; }
	bool IsActive(){ return false;  }	
	bool MustUpdate() { return false; }	

	void Update();
	void Execute();
};

#endif
