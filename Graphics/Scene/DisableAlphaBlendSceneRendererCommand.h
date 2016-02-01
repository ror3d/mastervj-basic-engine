#ifndef DISABLE_APLHA_BLEND_SCENE_RENDERER_COMMAND_H
#define DISABLE_APLHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CDisableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
	CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif