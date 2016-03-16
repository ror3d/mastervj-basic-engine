#ifndef POOL_RENDERABLE_OBJECT_TECHNIQUE_H
#define POOL_RENDERABLE_OBJECT_TECHNIQUE_H

#include "Utils/Named.h"
#include "Renderable/RenderableObjectTechnique.h"

#include <vector>

class CXMLTreeNode;

class CPoolRenderableObjectTechnique : public CNamed
{
private:
	class CPoolRenderableObjectTechniqueElement
	{
	public:
		TMapManager<CRenderableObjectTechnique>::Ref m_RenderableObjectTechnique;
		TMapManager<CRenderableObjectTechnique>::Ref m_OnRenderableObjectTechniqueManager;
		CPoolRenderableObjectTechniqueElement(TMapManager<CRenderableObjectTechnique>::Ref renderableObjectTechnique, TMapManager<CRenderableObjectTechnique>::Ref OnRenderableObjectTechniqueManager);
		~CPoolRenderableObjectTechniqueElement() { }
	};
	std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;
public:
	CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CPoolRenderableObjectTechnique();
	void Destroy();
	void AddElement(const std::string &Name, const std::string &TechniqueName, TMapManager<CRenderableObjectTechnique>::Ref ROTOnRenderableObjectTechniqueManager);
	void Apply();
};

#endif