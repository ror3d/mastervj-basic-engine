#include "Renderable/PoolRenderableObjectTechnique.h"
#include "Engine/Engine.h"


CPoolRenderableObjectTechnique
	::CPoolRenderableObjectTechniqueElement
		::CPoolRenderableObjectTechniqueElement(const std::string &Name,
	CEffectTechnique *EffectTechnique, CRenderableObjectTechnique
	*OnRenderableObjectTechniqueManager)
	: m_OnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager)
	, m_RenderableObjectTechnique(Name, EffectTechnique)
{
}


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode){
	setName(TreeNode.GetPszProperty("name"));
}
CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique(){
	
}
void CPoolRenderableObjectTechnique::Destroy(){
	m_RenderableObjectTechniqueElements.clear();
	delete(&m_RenderableObjectTechniqueElements);
}
void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName,
	CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager){
	
	m_RenderableObjectTechniqueElements.push_back(
		new CPoolRenderableObjectTechniqueElement(
			Name,
			CEngine::GetSingleton().getEffectsManager()->get(TechniqueName), 
			ROTOnRenderableObjectTechniqueManager));

}
void CPoolRenderableObjectTechnique::Apply(){
	for (auto elem : m_RenderableObjectTechniqueElements)
	{
		elem->m_OnRenderableObjectTechniqueManager
			->SetEffectTechnique(elem->m_RenderableObjectTechnique.GetEffectTechnique());
	}
}
