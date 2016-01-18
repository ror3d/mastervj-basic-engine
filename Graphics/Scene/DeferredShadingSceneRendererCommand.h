#ifndef DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H
#define DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H

#include "Scene\StagedTexturedSceneRendererCommand.h"
#include "Renderable\RenderableObjectTechnique.h"

class CDeferredShadingSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	ID3D11BlendState *m_EnabledAlphaBlendState;
public:
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CDeferredShadingSceneRendererCommand();
	void Execute(CContextManager &_context);
};

#endif