#ifndef DRAW_QUAD_SCENE_RENDERER_COMMAND_H
#define DRAW_QUAD_SCENE_RENDERER_COMMAND_H

#include "Scene\StagedTexturedSceneRendererCommand.h"
#include "Renderable\RenderableObjectTechnique.h"

class CDrawQuadSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
public:
	CDrawQuadSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif