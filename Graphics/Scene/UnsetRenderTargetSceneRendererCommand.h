#ifndef UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand
{
public:
	CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif