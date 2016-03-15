#ifndef STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H
#define STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H

#include "Scene/SceneRendererCommand.h"
#include "Graphics/Texture/DynamicTexture.h"
#include <vector>
#include <Base/Utils/TMapManager.h>

class CStagedTexturedSceneRendererCommand : public CSceneRendererCommand
{
protected:
	class CStageTexture
	{
	public:
		int m_StageId;
		TMapManager<CTexture>::Ref m_Texture;
		CStageTexture(int StageId, TMapManager<CTexture>::Ref Texture)
		{
			m_Texture = std::move(Texture);
			m_StageId = StageId;
		}
	};
	std::vector<CStageTexture *> m_StageTextures;
	std::vector<CDynamicTexture *> m_DynamicTextures;
	std::vector<ID3D11RenderTargetView *> m_RenderTargetViews;
public:
	CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CStagedTexturedSceneRendererCommand();
	void CreateRenderTargetViewVector();
	void ActivateTextures();
	void AddStageTexture(int StageId, TMapManager<CTexture>::Ref Texture);
	virtual void Execute(CContextManager &context) = 0;
};

#endif