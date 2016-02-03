#ifndef PRESENT_SCENE_RENDERER_COMMAND_H
#define PRESENT_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CPresentSceneRendererCommand : public CSceneRendererCommand
{
public:
	CPresentSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};
#endif