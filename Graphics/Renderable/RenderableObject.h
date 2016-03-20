#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Graphics/Renderer/3DElement.h"
#include <Base/Utils/Named.h> 
#include <Core/Component/ComponentManager.h>


class CContextManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;
class CScriptedComponent;

class CRenderableObject : public C3DElement, public CNamed
{
public:

	CRenderableObject() : CNamed("") { m_componentManager = new CComponentManager(); }
	CRenderableObject(CXMLTreeNode& treeNode) : C3DElement(treeNode), CNamed(treeNode) { m_componentManager = new CComponentManager(); }

	CRenderableObjectTechnique * m_renderableObjectTechnique;
	CComponentManager * m_componentManager;

	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CContextManager *_context) = 0;
	CComponentManager * GetComponentManager(){ return m_componentManager;  }
	void AddComponent(std::string Name, CScriptedComponent * component);
};

#endif