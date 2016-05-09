#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Graphics/Renderer/3DElement.h"
#include <Base/Utils/Named.h>
#include <Base/Utils/TMapManager.h>


class CContextManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;
class CComponent;

class CRenderableObject : public C3DElement, public CNamed
{
public:
	typedef TMapManager<CComponent> ComponentContainer_t;

	CRenderableObject();
	CRenderableObject(CXMLTreeNode& treeNode);

	CRenderableObjectTechnique* m_renderableObjectTechnique;

	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CContextManager* _context) = 0;

	void AddComponent(std::string Name, CComponent* component);

	/*
	ComponentContainer_t* GetComponentManager()
	{
		return m_componentManager;
	}
	*/
private:
	ComponentContainer_t m_componentContainer;
};

#endif