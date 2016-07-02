#include "Scene/RenderLayerSceneRendererCommand.h"
//#include <Graphics/Layer/LayerManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Renderer/Renderer.h>

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{
	//TODO: save active
	//<render_layer layer="solid" active="true"/>
	std::string layerNameToRender = TreeNode.GetPszProperty("layer");
	m_layerName = layerNameToRender;
	/*m_Layer = CEngine::GetSingleton().getLayerManager()->get(layerNameToRender);
	if (m_Layer != nullptr)
	{
		m_Layer->setActive(TreeNode.GetBoolProperty("active"));
	}*/
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	/*if (m_Layer != nullptr)
	{
		m_Layer->Render(&_context);
	}*/
	CEngine::GetSingleton().getRenderer()->RenderLayer(m_layerName, &_context);
}