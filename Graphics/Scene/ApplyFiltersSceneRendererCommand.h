#ifndef APPLY_FILTERS_SCENE_RENDERER_COMMAND_H
#define APPLY_FILTERS_SCENE_RENDERER_COMMAND_H

#include "Scene/StagedTexturedSceneRendererCommand.h"
#include <vector>

class CApplyFiltersSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	std::vector<CMaterial*> m_Materials;

public:
	CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode);
	~CApplyFiltersSceneRendererCommand();
	virtual void Execute(CContextManager &_context);
};

#endif