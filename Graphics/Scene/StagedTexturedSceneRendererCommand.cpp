#include "Scene\StagedTexturedSceneRendererCommand.h"
#include <Engine/Engine.h>

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode)
	:CSceneRendererCommand(TreeNode)
{

}
CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand()
{
	CTextureManager * textureManager = CEngine::GetSingleton().getTextureManager();

	if (!textureManager->isEmpty())
	{
		for (int i = 0; i < m_DynamicTextures.size(); ++i)
		{
			textureManager->remove(m_DynamicTextures[i]->getName());
		}
	}

	m_DynamicTextures.clear();
	m_StageTextures.clear();
}

void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	/*El método CreateRenderTargetViewVector se llamará al terminar de leer el nodo
y se rellenará con todos los RenderTargets de las texturas dinámicas.*/
	for (int i = 0; i < m_DynamicTextures.size(); i++)
	{
		auto text = dynamic_cast<CDynamicTexture *>(m_DynamicTextures[i].getRef());
		m_RenderTargetViews.push_back(text->GetRenderTargetView());
	}
}

void CStagedTexturedSceneRendererCommand::ActivateTextures()
{
	for (int i = 0; i < m_StageTextures.size(); i++)
	{
		m_StageTextures[i]->m_Texture->Activate(m_StageTextures[i]->m_StageId);
	}
}

void CStagedTexturedSceneRendererCommand::AddStageTexture(int StageId, TMapManager<CTexture>::Ref Texture)
{
	m_StageTextures.push_back(new CStageTexture(StageId, std::move(Texture)));
}

