#ifndef IA_INSTANCE_H
#define IA_INSTANCE_H

#include "Utils/Named.h"

#include <Base/Math/Math.h>

class CRenderableObject;

class CIAInstance : public CNamed
{
private:
	Vect3f m_target;
	Vect3f m_rotTarget;

	float m_anglesToTarget;
	Vect3f m_lastPosition;
	Vect3f m_lastRotation;
	float m_distToTarget;

	float m_actualTime;
	float m_velToRotate; //rads/s
	float m_velToMove; //m/s

	bool rotRight;

public:
	CIAInstance(std::string Name, float timeToUp);
	virtual ~CIAInstance();

	CRenderableObject * m_IAOb;
	float m_timeToUpdate;

	void Update(float elapsedTime);
	void UpdateTarget();
	void GetTarget();


};

#endif
