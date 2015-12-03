#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "3DElement.h"
#include "Utils\Named.h"

class CRenderableObject : public C3DElement, public CNamed
{
public:
	CRenderableObject() : CNamed("") {}
	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CRenderManager *RM) = 0;
};

#endif