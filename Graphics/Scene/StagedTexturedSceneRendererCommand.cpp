#include "Scene\StagedTexturedSceneRendererCommand.h"

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode)
	:CSceneRendererCommand(TreeNode)
{

}
CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand()
{
}

void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	/*El método CreateRenderTargetViewVector se llamará al terminar de leer el nodo
y se rellenará con todos los RenderTargets de las texturas dinámicas.*/
	for (auto t : m_DynamicTextures)
	{
		m_RenderTargetViews.push_back(t->GetRenderTargetView());
	}
}

void CStagedTexturedSceneRendererCommand::ActivateTextures()
{
	for (int i = 0; i < m_StageTextures.size(); i++)
	{
		m_StageTextures[i].m_Texture->Activate(m_StageTextures[i].m_StageId);
	}
}

void CStagedTexturedSceneRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
	m_StageTextures.push_back(CStageTexture(StageId, Texture));
}

