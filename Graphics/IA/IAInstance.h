#ifndef IA_INSTANCE_H
#define IA_INSTANCE_H

#include "Utils/Named.h"

class CRenderableObject;

class CIAInstance : public CNamed
{


public:
	CIAInstance(std::string Name, float timeToUp);
	virtual ~CIAInstance();
	void Update();
	CRenderableObject * m_IAOb;
	float timeToUpdate;

};

#endif