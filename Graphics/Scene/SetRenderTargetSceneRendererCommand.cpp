#include "Scene\SetRenderTargetSceneRendererCommand.h"

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CStagedTexturedSceneRendererCommand(TreeNode){

}


//<dynamic_texture name="DiffuseMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8" create_depth_stencil_buffer="false"/>
void CSetRenderTargetSceneRendererCommand::Execute(CContextManager &_context)
{
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : m_DynamicTextures[0]->GetDepthStencilView();
	_context.SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0], l_DepthStencilView == NULL ?	_context.GetDepthStencilView() : l_DepthStencilView);
}