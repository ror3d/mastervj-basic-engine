#include "Renderable/PoolRenderableObjectTechnique.h"
#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string &Name, TMapManager<CEffectTechnique>::Ref EffectTechnique, TMapManager<CRenderableObjectTechnique>::Ref OnRenderableObjectTechniqueManager)
	: m_OnRenderableObjectTechniqueManager(std::move(OnRenderableObjectTechniqueManager))
	, m_RenderableObjectTechnique(Name, std::move(EffectTechnique))
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
	m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(Name, CEngine::GetSingleton().getEffectsManager()->get(TechniqueName), CEngine::GetSingleton().getRenderableObjectTechniqueManager()->get(ROTOnRenderableObjectTechniqueManager->getName())));
}

void CPoolRenderableObjectTechnique::Apply()
{
	for (auto elem : m_RenderableObjectTechniqueElements)
	{
		elem->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(CEngine::GetSingleton().getEffectsManager()->get(elem->m_RenderableObjectTechnique.GetEffectTechnique()->getName()));
	}
}
