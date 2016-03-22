#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

class CTimerManager
{

public:
	CTimerManager(){}
	virtual ~CTimerManager(){}
	float m_elapsedTime;

	float getElapsedTime(){ return m_elapsedTime;  }

	void destroy() {}
};

#endif