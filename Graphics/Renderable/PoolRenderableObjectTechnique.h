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
		CRenderableObjectTechnique m_RenderableObjectTechnique;
		TMapManager<CRenderableObjectTechnique>::Ref m_OnRenderableObjectTechniqueManager;
		CPoolRenderableObjectTechniqueElement(const std::string &Name, TMapManager<CEffectTechnique>::Ref EffectTechnique, TMapManager<CRenderableObjectTechnique>::Ref OnRenderableObjectTechniqueManager);
		~CPoolRenderableObjectTechniqueElement() { }
	};
	std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;
public:
	CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CPoolRenderableObjectTechnique();
	void Destroy();
	void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
	void Apply();
};

#endif