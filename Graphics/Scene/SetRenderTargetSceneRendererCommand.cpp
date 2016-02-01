#include "Scene\SetRenderTargetSceneRendererCommand.h"

void CSetRenderTargetSceneRendererCommand::Execute(CContextManager &_context)
{
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : m_DynamicTextures[0]->GetDepthStencilView();
	_context.SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0], l_DepthStencilView == NULL ?	_context.GetDepthStencilView() : l_DepthStencilView);
}