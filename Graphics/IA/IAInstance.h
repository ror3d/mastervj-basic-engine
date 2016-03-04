#ifndef IA_INSTANCE_H
#define IA_INSTANCE_H

#include "Utils/Named.h"

class CRenderableObject;

class CIAInstance : public CNamed
{
private:
	Vect3f m_target;
	float m_timeToReachTarget;
	float m_actualTime;

public:
	CIAInstance(std::string Name, float timeToUp);
	virtual ~CIAInstance();
	void Update(float elapsedTime);
	CRenderableObject * m_IAOb;
	float m_timeToUpdate;
	void RecalculateTarget();
	void GetTarget();

};

#endif
