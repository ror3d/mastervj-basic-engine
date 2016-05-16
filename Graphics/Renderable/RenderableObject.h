#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Graphics/Renderer/3DElement.h"
#include <Base/Utils/Named.h>
#include <Base/Utils/TMapManager.h>


class CContextManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;
class CComponent;
class CCharacterControllerComponent;
class CAnimatedInstanceModel;

class CRenderableObject : public C3DElement, public CNamed
{
public:
	typedef TMapManager<CComponent> ComponentContainer_t;

	CRenderableObject();
	CRenderableObject(CXMLTreeNode& treeNode);

	CRenderableObjectTechnique* m_renderableObjectTechnique;

	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CContextManager* _context) { throw std::exception("You shall not use this!"); };

	void AddComponent(std::string Name, CComponent* component);

	void SendMsg(const std::string message);

	CCharacterControllerComponent* GetCharacterController();

	virtual CAnimatedInstanceModel* AsAnimatedInstance() { return nullptr; }

private:
	ComponentContainer_t m_componentContainer;
};

#endif