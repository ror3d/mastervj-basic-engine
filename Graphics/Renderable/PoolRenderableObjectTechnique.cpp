#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Engine\Engine.h"

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
	for (std::vector<CPoolRenderableObjectTechniqueElement * >::iterator it = m_RenderableObjectTechniqueElements.begin();
		it != m_RenderableObjectTechniqueElements.end(); ++it)
	{
		CPoolRenderableObjectTechniqueElement *l_PoolRenderableObjectTechniqueElement = (*it);
		l_PoolRenderableObjectTechniqueElement->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(l_PoolRenderableObjectTechniqueElement->m_RenderableObjectTechnique.GetEffectTechnique());
	}
}
