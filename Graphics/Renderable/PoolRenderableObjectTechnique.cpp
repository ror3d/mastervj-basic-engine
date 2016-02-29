#include "Renderable/PoolRenderableObjectTechnique.h"
#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>


CPoolRenderableObjectTechnique
	::CPoolRenderableObjectTechniqueElement
		::CPoolRenderableObjectTechniqueElement(const std::string &Name,
	CEffectTechnique *EffectTechnique, CRenderableObjectTechnique
	*OnRenderableObjectTechniqueManager)
	: m_OnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager)
	, m_RenderableObjectTechnique(Name, EffectTechnique)
{
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

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
	m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(Name, CEngine::GetSingleton().getEffectsManager()->get(TechniqueName), ROTOnRenderableObjectTechniqueManager));
}

void CPoolRenderableObjectTechnique::Apply()
{
	for (auto elem : m_RenderableObjectTechniqueElements)
	{
		elem->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(elem->m_RenderableObjectTechnique.GetEffectTechnique());
	}
}
