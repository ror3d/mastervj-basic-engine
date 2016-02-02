#ifndef RENDER_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_LAYER_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Engine\Engine.h"

class CRenderLayerSceneRendererCommand : public CSceneRendererCommand
{
private:
	CRenderableObjectsManager *m_Layer;
public:
	CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif