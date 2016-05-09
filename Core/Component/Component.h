#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/Named.h"

class CRenderableObject;
class CContextManager;

class CComponent : public CNamed
{
private:
	CRenderableObject* m_Owner;

public:
	CComponent(const std::string& Name, CRenderableObject* Owner)
		: CNamed(Name)
		, m_Owner(Owner)
	{
	}

	virtual ~CComponent() {}

	virtual void Update(float ElapsedTime) = 0;
	virtual void Render(CContextManager&  _context) = 0;
	virtual void RenderDebug(CContextManager&  _context) = 0;

	virtual void Destroy() {}

	CRenderableObject* GetOwner()
	{
		return m_Owner;
	}
};

#endif
