#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
}

#endif