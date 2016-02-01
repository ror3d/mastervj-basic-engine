#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Engine\Engine.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode){
	setName(TreeNode.GetPszProperty("pool"));
}
CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique(){
	
}
void CPoolRenderableObjectTechnique::Destroy(){
	m_RenderableObjectTechniqueElements.clear();
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
	/*TODO: recorreremos el pool y estableceremos las CEffectTechnique de
		m_RenderableObjectTechnique sobre el
		m_OnRenderableObjectTechniqueManager de los elementos.*/
	for (std::vector<CPoolRenderableObjectTechniqueElement * >::iterator it = m_RenderableObjectTechniqueElements.begin();
		it != m_RenderableObjectTechniqueElements.end(); ++it)
	{		
		//it->second->m_OnRenderableObjectTechniqueManager.SetEffectTechnique(m_RenderableObjectTechnique.GetEffectTechnique() ));
	}
}