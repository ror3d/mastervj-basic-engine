#ifndef SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_RENDERER_COMMAND_H
#define SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"
#include "Renderable\PoolRenderableObjectTechnique.h"

class CSetPoolRenderableObjectsTechniqueSceneRendererCommand : public
	CSceneRendererCommand
{
private:
	TMapManager<CPoolRenderableObjectTechnique>::Ref m_Pool;
public:
	CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif