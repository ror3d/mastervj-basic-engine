#ifndef RENDER_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_LAYER_SCENE_RENDERER_COMMAND_H

#include "Scene/StagedTexturedSceneRendererCommand.h"

class CRenderLayerSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	std::string m_layerName;
	bool m_zSort;
public:
	CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif