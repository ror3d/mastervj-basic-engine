#include "Scene/RenderLayerSceneRendererCommand.h"
//#include <Graphics/Layer/LayerManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Renderer/Renderer.h>

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{
	//TODO: save active
	std::vector<std::string> Props = TreeNode.GetProperties();
	for (int i = 0; i < Props.size(); i++)
	{
		if (Props[i] == "layer")
		{
			std::string layerNameToRender = TreeNode.GetPszProperty("layer");
			m_layerName = layerNameToRender;
		}
		
		if (Props[i] == "z_sort")
		{
			m_layer_zsort = TreeNode.GetBoolProperty("z_sort", false, true);
		} 
	}
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	/*if (m_Layer != nullptr)
	{
		m_Layer->Render(&_context);
	}*/
	CEngine::GetSingleton().getRenderer()->RenderLayer(m_layerName, &_context, m_layer_zsort);
}