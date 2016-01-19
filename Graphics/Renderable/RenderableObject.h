#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Renderer/3DElement.h"
#include <Base/Utils/Named.h>
#include "RenderableObjectTechnique.h"

class CContextManager;
class CXMLTreeNode;

class CRenderableObject : public C3DElement, public CNamed
{
public:

	CRenderableObject() : CNamed("") {}
	CRenderableObject(CXMLTreeNode& treeNode) : CNamed(treeNode) {}

	CRenderableObjectTechnique * m_renderableObjectTechnique;

	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CContextManager *_context) = 0;
};

#endif