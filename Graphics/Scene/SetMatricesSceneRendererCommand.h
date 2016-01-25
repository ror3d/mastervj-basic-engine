#ifndef SET_MATERICES_SCENE_RENDERER_COMMAND_H
#define SET_MATERICES_SCENE_RENDERER_COMMAND_H

#include <Scene\SceneRendererCommand.h>

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif