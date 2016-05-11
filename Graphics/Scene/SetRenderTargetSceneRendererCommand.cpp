#include "Scene/SetRenderTargetSceneRendererCommand.h"
#include "Engine/Engine.h"

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CStagedTexturedSceneRendererCommand(TreeNode)
{
	setName(TreeNode.GetPszProperty("name"));
	for (int i = 0; i < TreeNode.GetNumChildren(); i++){
		CXMLTreeNode text = TreeNode(i);
		if (text.GetName()==std::string("dynamic_texture"))
		{
			//XML SceneRenderCommands:
			//<dynamic_texture name="DiffuseMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8" create_depth_stencil_buffer="false"/>
			CDynamicTexture * dynText = new CDynamicTexture(text);
			CEngine::GetSingleton().getTextureManager()->add(dynText->getName(), dynText);
			m_DynamicTextures.push_back(CEngine::GetSingleton().getTextureManager()->get(dynText->getName()));
		}
	}

	CreateRenderTargetViewVector();
}

CSetRenderTargetSceneRendererCommand::~CSetRenderTargetSceneRendererCommand()
{
	// TODO
}


void CSetRenderTargetSceneRendererCommand::Execute(CContextManager &_context)
{
	auto text = dynamic_cast<CDynamicTexture *>(m_DynamicTextures[0].getRef());
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : text->GetDepthStencilView();
	_context.SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0], l_DepthStencilView == NULL ? _context.GetDepthStencilView() : l_DepthStencilView);
}