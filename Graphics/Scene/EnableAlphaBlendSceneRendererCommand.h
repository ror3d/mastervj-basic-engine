#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_context);
}

#endif