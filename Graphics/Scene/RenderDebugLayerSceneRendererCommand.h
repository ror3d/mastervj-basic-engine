#ifndef RENDER_DEBUG_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_LAYER_SCENE_RENDERER_COMMAND_H

#include "Scene/SceneRendererCommand.h"

class CRenderDebugLayerSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif