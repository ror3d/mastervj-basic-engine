#ifndef SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "Scene\StagedTexturedSceneRendererCommand.h"

class CSetRenderTargetSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
public:
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode);
	~CSetRenderTargetSceneRendererCommand();
	void Execute(CContextManager &_context);
};

#endif