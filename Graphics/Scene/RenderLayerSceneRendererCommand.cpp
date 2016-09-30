#include "Scene/RenderLayerSceneRendererCommand.h"
//#include <Graphics/Layer/LayerManager.h>
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Renderer/Renderer.h>

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
	, m_zSort(false)
{
	m_layerName = TreeNode.GetPszProperty("layer");

	m_zSort = TreeNode.GetBoolProperty("z_sort", false, false);
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	CEngine::GetSingleton().getRenderer()->RenderLayer(m_layerName, &_context, m_zSort);
}