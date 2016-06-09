#include "Renderable/PoolRenderableObjectTechnique.h"
#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Effect/EffectTechnique.h>


CPoolRenderableObjectTechnique
	::CPoolRenderableObjectTechniqueElement
	::CPoolRenderableObjectTechniqueElement(const std::string &Name,
											TMapManager<CEffectTechnique>::Ref EffectTechnique,
											CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
	: m_OnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager)
	, m_RenderableObjectTechnique(EffectTechnique->getName(), std::move(EffectTechnique))
{
	auto name = m_RenderableObjectTechnique.GetEffectTechnique()->getName();
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
	m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(Name, CEngine::GetSingleton().getEffectsManager()->ref(TechniqueName), ROTOnRenderableObjectTechniqueManager));
}

void CPoolRenderableObjectTechnique::Apply()
{
	for (auto elem : m_RenderableObjectTechniqueElements)
	{
		elem->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(CEngine::GetSingleton().getEffectsManager()->ref(elem->m_RenderableObjectTechnique.getName()));
	}
}
