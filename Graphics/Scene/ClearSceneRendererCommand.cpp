#include "Scene/ClearSceneRendererCommand.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
	, m_ClearColor(false)
{
	m_RenderTarget = TreeNode.GetBoolProperty("render_target");
	m_DepthStencil = TreeNode.GetBoolProperty("depth_stencil");
	Vect3f c = TreeNode.GetVect3fProperty("color", Vect3f(-1, -1, -1), false);
	if (c.x >= 0)
	{
		m_color.Set(c.x, c.y, c.z, 1.0);
		m_ClearColor = true;
	}
}

void CClearSceneRendererCommand::Execute(CContextManager &_contex)
{
	if (m_ClearColor)
	{
		auto c = _contex.m_BackgroundColor;
		_contex.m_BackgroundColor = m_color;
		_contex.Clear(m_DepthStencil, m_RenderTarget);
		_contex.m_BackgroundColor = c;
	}
	else
	{
		_contex.Clear(m_DepthStencil, m_RenderTarget);
	}
}