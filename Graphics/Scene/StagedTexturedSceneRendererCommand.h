#ifndef STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H
#define STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CStagedTexturedSceneRendererCommand : public CSceneRendererCommand
{
protected:
	class CStageTexture
	{
	public:
		int m_StageId;
		CTexture *m_Texture;
		CStageTexture(int StageId, CTexture *Texture)
		{
			m_StageId = StageId;
			m_Texture = Texture;
		}
		void Activate();
	};
	std::vector<CStageTexture> m_StageTextures;
	//TODO:
	//std::vector<CDynamicTexture *> m_DynamicTextures;
	std::vector<ID3D11RenderTargetView *> m_RenderTargetViews;
public:
	CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CStagedTexturedSceneRendererCommand();
	void CreateRenderTargetViewVector();
	void ActivateTextures();
	void AddStageTexture(int StageId, CTexture *Texture);
	virtual void Execute(CContextManager &context) = 0;
};

#endif