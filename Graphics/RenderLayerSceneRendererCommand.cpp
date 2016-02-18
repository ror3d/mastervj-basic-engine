#include "Scene/RenderLayerSceneRendererCommand.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){
	//TODO: save active
	//<render_layer layer="solid" active="true"/>
	std::string layerNameToRender = TreeNode.GetPszProperty("layer");
	m_Layer = CEngine::GetSingleton().getLayerManager()->get(layerNameToRender);	
	m_Layer->setActive(TreeNode.GetBoolProperty("active"));
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	m_Layer->Render(&_context);
}