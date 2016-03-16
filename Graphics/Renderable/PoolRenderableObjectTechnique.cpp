#include "Renderable/PoolRenderableObjectTechnique.h"
#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(TMapManager<CRenderableObjectTechnique>::Ref renderableObjectTechnique, TMapManager<CRenderableObjectTechnique>::Ref OnRenderableObjectTechniqueManager)
{
	m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
	m_RenderableObjectTechnique = renderableObjectTechnique;
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
	setName(TreeNode.GetPszProperty("name"));
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
	Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
	for (auto rote : m_RenderableObjectTechniqueElements)
	{
		delete rote;
	}

	m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, TMapManager<CRenderableObjectTechnique>::Ref ROTOnRenderableObjectTechniqueManager)
{
	m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(CEngine::GetSingleton().getRenderableObjectTechniqueManager()->get(Name), std::move(ROTOnRenderableObjectTechniqueManager)));
}

void CPoolRenderableObjectTechnique::Apply()
{
	for (size_t i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i)
	{
		auto elem = m_RenderableObjectTechniqueElements[i];
		std::string name = elem->m_RenderableObjectTechnique.getRef()->GetEffectTechnique()->getName();
		auto effect = CEngine::GetSingleton().getEffectsManager()->get(name);
		elem->m_OnRenderableObjectTechniqueManager.getRef()->SetEffectTechnique(std::move(effect));
	}
}
