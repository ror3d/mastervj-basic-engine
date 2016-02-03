#ifndef RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"
#include "Renderable\RenderableObjectTechnique.h"

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
private:
	float m_Width;
	float m_Offset;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
public:
	CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CContextManager &_context);
};

#endif