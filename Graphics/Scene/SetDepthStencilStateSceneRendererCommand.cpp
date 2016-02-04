#include "Graphics\Scene\SetDepthStencilStateSceneRendererCommand.h"

CSetDepthStencilStateSceneRendererCommand::CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode)
: CSceneRendererCommand(TreeNode)
{
	D3D11_DEPTH_STENCIL_DESC l_DepthStencilStateDescription;
	ZeroMemory(&l_DepthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
	l_DepthStencilStateDescription.DepthEnable = TreeNode.GetBoolProperty("enable_z_test", true) ? TRUE : FALSE;
	l_DepthStencilStateDescription.DepthWriteMask = TreeNode.GetBoolProperty("write_z_buffer", true) ? D3D11_DEPTH_WRITE_MASK_ALL :	D3D11_DEPTH_WRITE_MASK_ZERO;
	l_DepthStencilStateDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	l_DepthStencilStateDescription.StencilEnable = TreeNode.GetBoolProperty("enable_stencil", true) ? TRUE : FALSE;
	HRESULT hr = CEngine::GetSingleton().getContextManager()->GetDevice()->CreateDepthStencilState(&l_DepthStencilStateDescription, &m_DepthStencilState);
	assert(hr);
}

CSetDepthStencilStateSceneRendererCommand::~CSetDepthStencilStateSceneRendererCommand(){

}


void CSetDepthStencilStateSceneRendererCommand::Execute(CContextManager	&_context)
{
	_context.GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, 0);
}