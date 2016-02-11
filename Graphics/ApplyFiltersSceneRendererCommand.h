#ifndef APPLY_FILTERS_SCENE_RENDERER_COMMAND_H
#define APPLY_FILTERS_SCENE_RENDERER_COMMAND_H

#include "Scene/StagedTexturedSceneRendererCommand.h"

class CApplyFiltersSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
public:
	CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_context);
};

#endif