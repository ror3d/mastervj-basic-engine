#include "Scene/RenderLayerSceneRendererCommand.h"
#include <Base/XML/XMLTreeNode.h>
#include "Engine/Engine.h"
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Renderer/Renderer.h>

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CStagedTexturedSceneRendererCommand(TreeNode)
	, m_zSort(false)
{
	m_layerName = TreeNode.GetPszProperty("layer");

	m_zSort = TreeNode.GetBoolProperty("z_sort", false, false);

	for (int i = 0; i < TreeNode.GetNumChildren(); i++)
	{
		CXMLTreeNode texChild = TreeNode(i);
		if (texChild.GetName() == std::string("texture"))
		{
			CTexture *tex = nullptr;
			if (texChild.GetBoolProperty("load_file", false))
			{
				tex = CEngine::GetSingleton().getTextureManager()->GetTexture(texChild.GetPszProperty("file"));
			}
			else
			{
				tex = CEngine::GetSingleton().getTextureManager()->get(texChild.GetPszProperty("file"));
			}
			AddStageTexture(texChild.GetIntProperty("stage_id"), tex);
		}
	}
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	ActivateTextures();
	CEngine::GetSingleton().getRenderer()->RenderLayer(m_layerName, &_context, m_zSort);
}