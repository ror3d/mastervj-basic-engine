#ifndef RENDER_DEBUG_GUI_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_GUI_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CRenderDebugGUISceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugGUISceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_contex);
};

#endif