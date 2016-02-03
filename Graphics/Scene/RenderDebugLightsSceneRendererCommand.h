#ifndef RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CRenderDebugLightsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_context);
};

#endif