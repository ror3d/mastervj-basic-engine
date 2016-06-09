#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Graphics/Renderer/3DElement.h"
#include <Base/Utils/Named.h>


class CContextManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;

class CRenderableObject : public C3DElement, public CNamed
{
public:
	CRenderableObject();
	CRenderableObject(CXMLTreeNode& treeNode);

	CRenderableObjectTechnique* m_renderableObjectTechnique;

	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CContextManager* _context) { throw std::exception("You shall not use this!"); };

private:
};

#endif