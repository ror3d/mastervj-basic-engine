#ifndef DRAW_QUAD_SCENE_RENDERER_COMMAND_H
#define DRAW_QUAD_SCENE_RENDERER_COMMAND_H

#include "Scene/StagedTexturedSceneRendererCommand.h"

class CMaterial;

class CDrawQuadSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
public:
	CDrawQuadSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif