#include "Scene\ClearSceneRendererCommand.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode){
	m_RenderTarget = TreeNode.GetBoolProperty("render_target");
	m_DepthStencil = TreeNode.GetBoolProperty("depth_stencil");
}

void CClearSceneRendererCommand::Execute(CContextManager &_contex){
	_contex.Clear(m_DepthStencil, m_RenderTarget);
}