#ifndef RENDER_DEBUG_GRID_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_GRID_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CRenderDebugGridSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugGridSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif